#include <vector>

#include "Lexer.h"
#include "Grammar.h"
#include "VirtualMachine.h"

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


void LiteralExpression::generateBytecode(std::vector<VM::Byte> &buffer) {
    switch(this->value.type) {
    case Lexing::TokenType::NUMBER: {
        buffer.push_back(VM::InstructionType::INT64_LOAD);
        buffer.push_back(parseInt64(this->value.lexeme));
        break;
    }
    case Lexing::TokenType::BOOLEAN: {
        buffer.push_back(VM::InstructionType::INT64_LOAD);
        // TODO: change this function to something more robust
        buffer.push_back(parseBool(this->value.lexeme));
        break;
    }
    default:{
        GrammarBytecodeError("Unsupported type of literal: ", this->value.type);
    }
    }
}   

void BinaryExpression::generateBytecode(std::vector<VM::Byte> &buffer) {
    GrammarBytecodeError("Binary expression bytecode generation is not currently supported");
}

void UnaryExpression::generateBytecode(std::vector<VM::Byte> &buffer) {
    GrammarBytecodeError("Unary expression bytecode generation is not currently supported");
}

void FunctionCall::generateBytecode(std::vector<VM::Byte> &buffer) {
    if(this->name == "print") {
        for(const auto &args : this->parameters) {
            args->generateBytecode(buffer);
            buffer.push_back(VM::InstructionType::PRINT);
        }
    } else {
        GrammarBytecodeError("Function call generation failed: only print is currently handled");
    }
}

void ExpressionStatement::generateBytecode(std::vector<VM::Byte> &buffer) {
    this->expr->generateBytecode(buffer);
}

void StatementList::generateBytecode(std::vector<VM::Byte> &buffer) {
    for(const auto &stmt : this->list) {
        stmt->generateBytecode(buffer);
    }
}

void IfStatement::generateBytecode(std::vector<VM::Byte> &buffer) {
    GrammarBytecodeError("If statement bytecode generation is not currently supported");
}

void DeclarationStatement::generateBytecode(std::vector<VM::Byte> &buffer) {
    GrammarBytecodeError("Declaration statement bytecode generation is not currently supported");
}

};
