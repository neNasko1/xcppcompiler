#include <vector>

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


void LiteralExpression::generateBytecode(std::vector<VM::Byte> &buffer, Context &ctx) {
    this->deduceType(ctx);

    switch(this->value.type) {
    case Lexing::TokenType::NUMBER: {
        buffer.push_back(VM::InstructionType::INT64_LOAD);
        buffer.push_back(parseInt64(this->value.lexeme));
        break;
    }
    case Lexing::TokenType::BOOLEAN: {
        buffer.push_back(VM::InstructionType::BOOL_LOAD);
        buffer.push_back(parseBool(this->value.lexeme));
        break;
    }
    case Lexing::TokenType::NAME: {
        auto currentVariable = ctx.findVariable(this->value.lexeme);
        switch(ctx.variables[currentVariable].type) {
        case TypeIndexes::INT64: {
            buffer.push_back(VM::InstructionType::INT64_LOAD_FROM_STACK);
            buffer.push_back(ctx.variables[currentVariable].offset);
            break;
        }
        case TypeIndexes::BOOL: {
            buffer.push_back(VM::InstructionType::BOOL_LOAD_FROM_STACK);
            buffer.push_back(ctx.variables[currentVariable].offset);
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

void BinaryExpression::generateBytecode(std::vector<VM::Byte> &buffer, Context &ctx) {
    this->deduceType(ctx);

    this->left->generateBytecode(buffer, ctx);
    this->right->generateBytecode(buffer, ctx);

    switch(this->operation) {
    case Lexing::TokenType::PLUS: {
        buffer.push_back(VM::InstructionType::INT64_ADD);
        break;
    }
    case Lexing::TokenType::MINUS: {
        buffer.push_back(VM::InstructionType::INT64_SUBTRACT);
        break;
    }
    case Lexing::TokenType::STAR: {
        buffer.push_back(VM::InstructionType::INT64_MULTIPLY);
        break;
    }
    case Lexing::TokenType::SLASH: {
        buffer.push_back(VM::InstructionType::INT64_DIVIDE);
        break;
    }
    case Lexing::TokenType::MODULO: {
        buffer.push_back(VM::InstructionType::INT64_MODULO);
        break;
    }
    case Lexing::TokenType::OR: {
        switch(this->type) {
            case Grammar::TypeIndexes::INT64: buffer.push_back(VM::InstructionType::INT64_OR); break;
            case Grammar::TypeIndexes::BOOL: buffer.push_back(VM::InstructionType::BOOL_OR); break;
        }
        break;
    }
    case Lexing::TokenType::OROR: {
        switch(this->type) {
            case Grammar::TypeIndexes::INT64: buffer.push_back(VM::InstructionType::INT64_OR); break;
            case Grammar::TypeIndexes::BOOL: buffer.push_back(VM::InstructionType::BOOL_OR); break;
        }
        break;
    }
    case Lexing::TokenType::AND: {
        switch(this->type) {
            case Grammar::TypeIndexes::INT64: buffer.push_back(VM::InstructionType::INT64_AND); break;
            case Grammar::TypeIndexes::BOOL: buffer.push_back(VM::InstructionType::BOOL_AND); break;
        }
        break;
    }
    case Lexing::TokenType::ANDAND: {
        switch(this->type) {
            case Grammar::TypeIndexes::INT64: buffer.push_back(VM::InstructionType::INT64_AND); break;
            case Grammar::TypeIndexes::BOOL: buffer.push_back(VM::InstructionType::BOOL_AND); break;
        }
        break;
    }
    case Lexing::TokenType::XOR: {
        switch(this->type) {
            case Grammar::TypeIndexes::INT64: buffer.push_back(VM::InstructionType::INT64_XOR); break;
            case Grammar::TypeIndexes::BOOL: buffer.push_back(VM::InstructionType::BOOL_XOR); break;
        }
        break;
    }
    case Lexing::TokenType::XORXOR: {
        switch(this->type) {
            case Grammar::TypeIndexes::INT64: buffer.push_back(VM::InstructionType::INT64_XOR); break;
            case Grammar::TypeIndexes::BOOL: buffer.push_back(VM::InstructionType::BOOL_XOR); break;
        }
        break;
    }
    case Lexing::TokenType::NOT: {
        buffer.push_back(VM::InstructionType::INT64_NOT);
        break;
    }
    case Lexing::TokenType::BANG: {
        buffer.push_back(VM::InstructionType::BOOL_NOT);
        break;
    }
    case Lexing::TokenType::EQUAL_EQUAL: {
        switch(this->type) {
            case Grammar::TypeIndexes::INT64: buffer.push_back(VM::InstructionType::INT64_EQUAL); break;
            case Grammar::TypeIndexes::BOOL: buffer.push_back(VM::InstructionType::BOOL_EQUAL); break;
        }
        break;
    }
    case Lexing::TokenType::BANG_EQUAL: {
        switch(this->type) {
            case Grammar::TypeIndexes::INT64: buffer.push_back(VM::InstructionType::INT64_NOT_EQUAL); break;
            case Grammar::TypeIndexes::BOOL: buffer.push_back(VM::InstructionType::BOOL_NOT_EQUAL); break;
        }
        break;
    }
    case Lexing::TokenType::SMALLER: {
        buffer.push_back(VM::InstructionType::INT64_SMALLER); 
        break;
    }
    case Lexing::TokenType::SMALLER_EQUAL: {
        buffer.push_back(VM::InstructionType::INT64_SMALLER_EQUAL); 
        break;
    }
    case Lexing::TokenType::BIGGER: {
        buffer.push_back(VM::InstructionType::INT64_BIGGER); 
        break;
    }
    case Lexing::TokenType::BIGGER_EQUAL: {
        buffer.push_back(VM::InstructionType::INT64_BIGGER_EQUAL); 
        break;
    }
    }
}

void UnaryExpression::generateBytecode(std::vector<VM::Byte> &buffer, Context &ctx) {
    this->deduceType(ctx);

    this->expr->generateBytecode(buffer, ctx);

    switch(this->operation) {
    case Lexing::TokenType::UNARY_PLUS: {
        break;
    }
    case Lexing::TokenType::UNARY_MINUS: {
        buffer.push_back(VM::InstructionType::INT64_NEGATE);
        break;
    }
    case Lexing::TokenType::NOT: {
        buffer.push_back(VM::InstructionType::INT64_NOT);
        break;
    }
    case Lexing::TokenType::BANG: {
        buffer.push_back(VM::InstructionType::BOOL_NOT);
        break;
    }
    default: {
        GrammarBytecodeError("Unary operation ", Lexing::TokenTypeName[this->operation], " is currently not supported");
    }
    }
}

void FunctionCall::generateBytecode(std::vector<VM::Byte> &buffer, Context &ctx) {
    this->deduceType(ctx);

    if(this->name == "print") {
        for(const auto &args : this->parameters) {
            args->generateBytecode(buffer, ctx);
            buffer.push_back(VM::InstructionType::PRINT);
        }
    } else if(this->name == "int64") {
        // Currently this is an edge case and this function is a cast so it has 1 parameter -> this was checked by deduceType so it's ok
        this->parameters[0]->generateBytecode(buffer, ctx);
        buffer.push_back(VM::InstructionType::BOOL_TO_INT64);
    } else if(this->name == "bool") {
        // Currently this is an edge case and this function is a cast so it has 1 parameter -> this was checked by deduceType so it's ok
        this->parameters[0]->generateBytecode(buffer, ctx);
        buffer.push_back(VM::InstructionType::INT64_TO_BOOL);
    } else {
        GrammarBytecodeError("Function call generation failed: only intrinsic functions are currently handled");
    }
}

void ExpressionStatement::generateBytecode(std::vector<VM::Byte> &buffer, Context &ctx) {
    this->expr->generateBytecode(buffer, ctx);
}

void StatementList::generateBytecode(std::vector<VM::Byte> &buffer, Context &ctx) {    
    for(const auto &stmt : this->list) {
        stmt->generateBytecode(buffer, ctx);
    }
}

void IfStatement::generateBytecode(std::vector<VM::Byte> &buffer, Context &ctx) {
    GrammarBytecodeError("If statement bytecode generation is not currently supported");
}

void DeclarationStatement::generateBytecode(std::vector<VM::Byte> &buffer, Context &ctx) {
    if(this->type == -1) {
        this->expr->deduceType(ctx);
        this->type = this->expr->type;
    }

    switch(this->type) {
    case TypeIndexes::INT64: {
        auto currentVariable = ctx.addVariable(this->name, this->type);
        if(!this->expr) {
            buffer.push_back(VM::InstructionType::INT64_LOAD);
            buffer.push_back(0);
        } else {
            this->expr->generateBytecode(buffer, ctx);
        }
        buffer.push_back(VM::InstructionType::INT64_LOAD_INTO_STACK);
        buffer.push_back(ctx.variables[currentVariable].offset);
        break;
    }
    case TypeIndexes::BOOL: {
        auto currentVariable = ctx.addVariable(this->name, this->type);
        if(!this->expr) {
            buffer.push_back(VM::InstructionType::BOOL_LOAD);
            buffer.push_back(false);
        } else {
            this->expr->generateBytecode(buffer, ctx);
        }
        buffer.push_back(VM::InstructionType::BOOL_LOAD_INTO_STACK);
        buffer.push_back(ctx.variables[currentVariable].offset);
        break;
    }
    default: {
        GrammarBytecodeError("Currently only int64 and bool types are supported");
        break;
    }
    }
}

};
