#include <vector>
#include <cassert>

#include "Lexer.h"
#include "Grammar.h"
#include "VirtualMachine.h"
#include "GrammarTypeChecking.h"
#include "GrammarBytecode.h"

namespace Grammar {

template <typename... T> 
static void GrammarBytecodeErrorPrint(T... t) {
    (std::cerr << ... << t) << "\n";
}

template <typename... T> 
void GrammarBytecodeError(T... t) {
    std::cerr << "There was an error while VM code was being generated " << "\n";
    GrammarBytecodeErrorPrint(t...);
    exit(0);
}

Variable::Variable(const std::string &_name, const uint64_t _type, const uint64_t _offset) : name(_name), type(_type), offset(_offset) {}
Variable::~Variable() {}

Context::Context() : variables(), variablesMapping() {
    this->offset = 0;
}
Context::~Context() {}

uint64_t Context::addVariable(const std::string &_name, const uint64_t _type) {
    this->variables.push_back(Variable(_name, _type, this->offset));
    this->variablesMapping[_name] = this->variables.size() - 1;
    // Move the stack pointer
    this->offset += Grammar::Type::globalTypes[_type].size;

    return this->variables.size() - 1;
}

uint64_t Context::findVariable(const std::string &name) {
    auto ptrToVariable = this->variablesMapping.find(name);
    
    if(ptrToVariable == this->variablesMapping.end()) {
        // Not found
        GrammarBytecodeError("Variable",  name, "is not declared");
        return -1;
    } else {
        return ptrToVariable->second;
    }
}

static int64_t parseInt64(const std::string &lexeme) {
    // TODO: change this function to something more robust
    int64_t ret = 0;
    for(const auto c : lexeme) {
        ret = (ret * 10ll) + (int64_t)(c - '0');
    }
    return ret;
}

static bool parseBool(const std::string &lexeme) {
    return lexeme == "true";   
}


void LiteralExpression::generateBytecode(VM::VirtualMachine::Code &buffer, Context &ctx) {
    this->deduceType(ctx);

    switch(this->value.type) {
    case Lexing::TokenType::NUMBER: {
        buffer.bytes.push_back(VM::InstructionType::INT64_LOAD);
        buffer.bytes.push_back(parseInt64(this->value.lexeme));
        break;
    }
    case Lexing::TokenType::BOOLEAN: {
        buffer.bytes.push_back(VM::InstructionType::BOOL_LOAD);
        buffer.bytes.push_back(parseBool(this->value.lexeme));
        break;
    }
    case Lexing::TokenType::NAME: {
        auto currentVariable = ctx.findVariable(this->value.lexeme);
        switch(ctx.variables[currentVariable].type) {
        case TypeIndexes::INT64: {
            buffer.bytes.push_back(VM::InstructionType::INT64_LOAD);
            buffer.bytes.push_back(ctx.variables[currentVariable].offset);
            buffer.bytes.push_back(VM::InstructionType::STACK_PTR_LOAD);
            buffer.bytes.push_back(VM::InstructionType::INT64_LOAD_FROM_ADDRESS);
            break;
        }
        case TypeIndexes::BOOL: {
            buffer.bytes.push_back(VM::InstructionType::INT64_LOAD);
            buffer.bytes.push_back(ctx.variables[currentVariable].offset);
            buffer.bytes.push_back(VM::InstructionType::STACK_PTR_LOAD);
            buffer.bytes.push_back(VM::InstructionType::BOOL_LOAD_FROM_ADDRESS);
            break;
        }
        default: {
            GrammarBytecodeError("Currently only int64 and bool types are supported");
            break;
        }
        }
        break;
    }
    default:{
        GrammarBytecodeError("Unsupported type of literal: ", this->value.lexeme, Lexing::TokenTypeName[this->value.type]);
        break;
    }
    }
}   

void BinaryExpression::generateBytecode(VM::VirtualMachine::Code &buffer, Context &ctx) {
    this->deduceType(ctx);

    this->left->generateBytecode(buffer, ctx);
    this->right->generateBytecode(buffer, ctx);

    switch(this->operation) {
    case Lexing::TokenType::PLUS: {
        buffer.bytes.push_back(VM::InstructionType::ADD);
        break;
    }
    case Lexing::TokenType::MINUS: {
        buffer.bytes.push_back(VM::InstructionType::SUBTRACT);
        break;
    }
    case Lexing::TokenType::STAR: {
        buffer.bytes.push_back(VM::InstructionType::MULTIPLY);
        break;
    }
    case Lexing::TokenType::SLASH: {
        buffer.bytes.push_back(VM::InstructionType::DIVIDE);
        break;
    }
    case Lexing::TokenType::MODULO: {
        buffer.bytes.push_back(VM::InstructionType::MODULO);
        break;
    }
    case Lexing::TokenType::OR: {
        switch(this->type) {
            case Grammar::TypeIndexes::INT64: buffer.bytes.push_back(VM::InstructionType::OR); break;
            case Grammar::TypeIndexes::BOOL: buffer.bytes.push_back(VM::InstructionType::OR); break;
        }
        break;
    }
    case Lexing::TokenType::OROR: {
        switch(this->type) {
            case Grammar::TypeIndexes::INT64: buffer.bytes.push_back(VM::InstructionType::OR); break;
            case Grammar::TypeIndexes::BOOL: buffer.bytes.push_back(VM::InstructionType::OR); break;
        }
        break;
    }
    case Lexing::TokenType::AND: {
        switch(this->type) {
            case Grammar::TypeIndexes::INT64: buffer.bytes.push_back(VM::InstructionType::AND); break;
            case Grammar::TypeIndexes::BOOL: buffer.bytes.push_back(VM::InstructionType::AND); break;
        }
        break;
    }
    case Lexing::TokenType::ANDAND: {
        switch(this->type) {
            case Grammar::TypeIndexes::INT64: buffer.bytes.push_back(VM::InstructionType::AND); break;
            case Grammar::TypeIndexes::BOOL: buffer.bytes.push_back(VM::InstructionType::AND); break;
        }
        break;
    }
    case Lexing::TokenType::XOR: {
        switch(this->type) {
            case Grammar::TypeIndexes::INT64: buffer.bytes.push_back(VM::InstructionType::XOR); break;
            case Grammar::TypeIndexes::BOOL: buffer.bytes.push_back(VM::InstructionType::XOR); break;
        }
        break;
    }
    case Lexing::TokenType::XORXOR: {
        switch(this->type) {
            case Grammar::TypeIndexes::INT64: buffer.bytes.push_back(VM::InstructionType::XOR); break;
            case Grammar::TypeIndexes::BOOL: buffer.bytes.push_back(VM::InstructionType::XOR); break;
        }
        break;
    }
    case Lexing::TokenType::NOT: {
        buffer.bytes.push_back(VM::InstructionType::NOT);
        break;
    }
    case Lexing::TokenType::BANG: {
        buffer.bytes.push_back(VM::InstructionType::NOT);
        break;
    }
    case Lexing::TokenType::EQUAL_EQUAL: {
        switch(this->type) {
            case Grammar::TypeIndexes::INT64: buffer.bytes.push_back(VM::InstructionType::EQUAL); break;
            case Grammar::TypeIndexes::BOOL: buffer.bytes.push_back(VM::InstructionType::EQUAL); break;
        }
        break;
    }
    case Lexing::TokenType::BANG_EQUAL: {
        switch(this->type) {
            case Grammar::TypeIndexes::INT64: buffer.bytes.push_back(VM::InstructionType::NOT_EQUAL); break;
            case Grammar::TypeIndexes::BOOL: buffer.bytes.push_back(VM::InstructionType::NOT_EQUAL); break;
        }
        break;
    }
    case Lexing::TokenType::SMALLER: {
        buffer.bytes.push_back(VM::InstructionType::SMALLER); 
        break;
    }
    case Lexing::TokenType::SMALLER_EQUAL: {
        buffer.bytes.push_back(VM::InstructionType::SMALLER_EQUAL); 
        break;
    }
    case Lexing::TokenType::BIGGER: {
        buffer.bytes.push_back(VM::InstructionType::BIGGER); 
        break;
    }
    case Lexing::TokenType::BIGGER_EQUAL: {
        buffer.bytes.push_back(VM::InstructionType::BIGGER_EQUAL); 
        break;
    }
    }
}

void UnaryExpression::generateBytecode(VM::VirtualMachine::Code &buffer, Context &ctx) {
    this->deduceType(ctx);

    this->expr->generateBytecode(buffer, ctx);

    switch(this->operation) {
    case Lexing::TokenType::UNARY_PLUS: {
        break;
    }
    case Lexing::TokenType::UNARY_MINUS: {
        buffer.bytes.push_back(VM::InstructionType::NEGATE);
        break;
    }
    case Lexing::TokenType::NOT: {
        buffer.bytes.push_back(VM::InstructionType::NOT);
        break;
    }
    case Lexing::TokenType::BANG: {
        buffer.bytes.push_back(VM::InstructionType::NOT);
        break;
    }
    default: {
        GrammarBytecodeError("Unary operation ", Lexing::TokenTypeName[this->operation], " is currently not supported");
    }
    }
}

void FunctionCall::generateBytecode(VM::VirtualMachine::Code &buffer, Context &ctx) {
    this->deduceType(ctx);

    if(this->name == "print") {
        for(const auto &args : this->parameters) {
            args->generateBytecode(buffer, ctx);
            buffer.bytes.push_back(VM::InstructionType::PRINT);
        }
    } else if(this->name == "int64") {
        // Currently this is an edge case and this function is a cast so it has 1 parameter -> this was checked by deduceType so it's ok
        this->parameters[0]->generateBytecode(buffer, ctx);
        buffer.bytes.push_back(VM::InstructionType::BOOL_TO_INT64);
    } else if(this->name == "bool") {
        // Currently this is an edge case and this function is a cast so it has 1 parameter -> this was checked by deduceType so it's ok
        this->parameters[0]->generateBytecode(buffer, ctx);
        buffer.bytes.push_back(VM::InstructionType::INT64_TO_BOOL);
    } else {
        GrammarBytecodeError("Function call generation failed: only intrinsic functions are currently handled");
    }
}

void ExpressionStatement::generateBytecode(VM::VirtualMachine::Code &buffer, Context &ctx) {
    this->expr->generateBytecode(buffer, ctx);
}

void StatementList::generateBytecode(VM::VirtualMachine::Code &buffer, Context &ctx) {    
    for(const auto &stmt : this->list) {
        stmt->generateBytecode(buffer, ctx);
    }
}

void IfStatement::generateBytecode(VM::VirtualMachine::Code &buffer, Context &ctx) {
    this->condition->deduceType(ctx);

    if(this->condition->type != Grammar::TypeIndexes::BOOL) {
        GrammarBytecodeError("If condition should evaluate to boolean");
    }

    this->condition->generateBytecode(buffer, ctx);

    // We have to jump_if_not
    buffer.bytes.push_back(VM::InstructionType::NOT);

    buffer.bytes.push_back(VM::InstructionType::JUMP_IF);
    buffer.bytes.push_back(buffer.lookupTable.size()); // Get the new position
    size_t indIfEnd = buffer.lookupTable.size(); // Save the index for later
    buffer.lookupTable.push_back(-1); // Some bad value, which we are going to swap later

    this->ifBody->generateBytecode(buffer, ctx);
    if(this->elseBody) { // If there is an else we have to do some jumping
        buffer.bytes.push_back(VM::InstructionType::JUMP);
        buffer.bytes.push_back(buffer.lookupTable.size()); // Get the new position
        size_t indElseEnd = buffer.lookupTable.size(); 
        buffer.lookupTable.push_back(-1); 

        buffer.lookupTable[indIfEnd] = buffer.bytes.size(); // This is the position we have to jump if the condition is false

        this->elseBody->generateBytecode(buffer, ctx);
        buffer.lookupTable[indElseEnd] = buffer.bytes.size();
    } else {
        buffer.lookupTable[indIfEnd] = buffer.bytes.size();
    }
}

void DeclarationStatement::generateBytecode(VM::VirtualMachine::Code &buffer, Context &ctx) {
    if(this->expr) {
        this->expr->deduceType(ctx);

        if(this->type == -1) {
            this->type = this->expr->type;
        } else if(this->expr->type != this->type) {
            GrammarBytecodeError("There was a type mismatch in declaration for variable ", this->name);
        }
    }
    
    switch(this->type) {
    case TypeIndexes::INT64: {
        auto currentVariable = ctx.addVariable(this->name, this->type);


        if(!this->expr) {
            buffer.bytes.push_back(VM::InstructionType::INT64_LOAD);
            buffer.bytes.push_back(0);
        } else {
            this->expr->generateBytecode(buffer, ctx);
        }

        buffer.bytes.push_back(VM::InstructionType::INT64_LOAD);
        buffer.bytes.push_back(ctx.variables[currentVariable].offset);
        buffer.bytes.push_back(VM::InstructionType::STACK_PTR_LOAD);
        buffer.bytes.push_back(VM::InstructionType::INT64_LOAD_INTO_ADDRESS);
        break;
    }
    case TypeIndexes::BOOL: {
        auto currentVariable = ctx.addVariable(this->name, this->type);

        if(!this->expr) {
            buffer.bytes.push_back(VM::InstructionType::BOOL_LOAD);
            buffer.bytes.push_back(false);
        } else {
            this->expr->generateBytecode(buffer, ctx);
        }

        buffer.bytes.push_back(VM::InstructionType::INT64_LOAD);
        buffer.bytes.push_back(ctx.variables[currentVariable].offset);
        buffer.bytes.push_back(VM::InstructionType::STACK_PTR_LOAD);
        buffer.bytes.push_back(VM::InstructionType::BOOL_LOAD_INTO_ADDRESS);
        break;
    }
    default: {
        GrammarBytecodeError("Currently only int64 and bool types are supported");
        break;
    }
    }
}

};
