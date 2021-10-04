#include <vector>

#include "Lexer.h"
#include "Grammar.h"
#include "VirtualMachine.h"
#include "GrammarTypeChecking.h"

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


void LiteralExpression::generateBytecode(std::vector<VM::Byte> &buffer) const {
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
    default:{
        GrammarBytecodeError("Unsupported type of literal: ", this->value.type);
    }
    }
}   

void BinaryExpression::generateBytecode(std::vector<VM::Byte> &buffer) const {
    this->left->generateBytecode(buffer);
    this->right->generateBytecode(buffer);

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
        switch(this->type->index) {
            case Grammar::TypeIndexes::INT64: buffer.push_back(VM::InstructionType::INT64_OR); break;
            case Grammar::TypeIndexes::BOOL: buffer.push_back(VM::InstructionType::BOOL_OR); break;
        }
        break;
    }
    case Lexing::TokenType::OROR: {
        switch(this->type->index) {
            case Grammar::TypeIndexes::INT64: buffer.push_back(VM::InstructionType::INT64_OR); break;
            case Grammar::TypeIndexes::BOOL: buffer.push_back(VM::InstructionType::BOOL_OR); break;
        }
        break;
    }
    case Lexing::TokenType::AND: {
        switch(this->type->index) {
            case Grammar::TypeIndexes::INT64: buffer.push_back(VM::InstructionType::INT64_AND); break;
            case Grammar::TypeIndexes::BOOL: buffer.push_back(VM::InstructionType::BOOL_AND); break;
        }
        break;
    }
    case Lexing::TokenType::ANDAND: {
        switch(this->type->index) {
            case Grammar::TypeIndexes::INT64: buffer.push_back(VM::InstructionType::INT64_AND); break;
            case Grammar::TypeIndexes::BOOL: buffer.push_back(VM::InstructionType::BOOL_AND); break;
        }
        break;
    }
    case Lexing::TokenType::XOR: {
        switch(this->type->index) {
            case Grammar::TypeIndexes::INT64: buffer.push_back(VM::InstructionType::INT64_XOR); break;
            case Grammar::TypeIndexes::BOOL: buffer.push_back(VM::InstructionType::BOOL_XOR); break;
        }
        break;
    }
    case Lexing::TokenType::XORXOR: {
        switch(this->type->index) {
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
        switch(this->type->index) {
            case Grammar::TypeIndexes::INT64: buffer.push_back(VM::InstructionType::INT64_EQUAL); break;
            case Grammar::TypeIndexes::BOOL: buffer.push_back(VM::InstructionType::BOOL_EQUAL); break;
        }
        break;
    }
    case Lexing::TokenType::BANG_EQUAL: {
        switch(this->type->index) {
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

void UnaryExpression::generateBytecode(std::vector<VM::Byte> &buffer) const {
    GrammarBytecodeError("Unary expression bytecode generation is not currently supported");
}

void FunctionCall::generateBytecode(std::vector<VM::Byte> &buffer) const {
    if(this->name == "print") {
        for(const auto &args : this->parameters) {
            args->generateBytecode(buffer);
            buffer.push_back(VM::InstructionType::PRINT);
        }
    } else if(this->name == "int64") {
        // Currently this is an edge case and this function is a cast so it has 1 parameter -> this was checked by deduceType so it's ok
        this->parameters[0]->generateBytecode(buffer);
        buffer.push_back(VM::InstructionType::BOOL_TO_INT64);
    } else if(this->name == "bool") {
        // Currently this is an edge case and this function is a cast so it has 1 parameter -> this was checked by deduceType so it's ok
        this->parameters[0]->generateBytecode(buffer);
        buffer.push_back(VM::InstructionType::INT64_TO_BOOL);
    } else {
        GrammarBytecodeError("Function call generation failed: only intrinsic functions are currently handled");
    }
}

void ExpressionStatement::generateBytecode(std::vector<VM::Byte> &buffer) const {
    this->expr->generateBytecode(buffer);
}

void StatementList::generateBytecode(std::vector<VM::Byte> &buffer) const {
    for(const auto &stmt : this->list) {
        stmt->generateBytecode(buffer);
    }
}

void IfStatement::generateBytecode(std::vector<VM::Byte> &buffer) const {
    GrammarBytecodeError("If statement bytecode generation is not currently supported");
}

void DeclarationStatement::generateBytecode(std::vector<VM::Byte> &buffer) const {
    GrammarBytecodeError("Declaration statement bytecode generation is not currently supported");
}

};
