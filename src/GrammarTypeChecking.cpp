#include <vector>
#include <map>
#include <string>

#include "Lexer.h"
#include "Grammar.h"
#include "VirtualMachine.h"
#include "GrammarTypeChecking.h"

namespace Grammar {

template <typename... T> 
static void TypeCheckingErrorPrint(T... t) {
    (std::cerr << ... << t) << "\n";
}

template <typename... T> 
void TypeCheckingError(T... t) {
    std::cerr << "There was a type mismatch error while VM code was being generated " << "\n";
    TypeCheckingErrorPrint(t...);
    exit(0);
}

Type::~Type() {}
Type::Type(const uint64_t _size, const int32_t _index, const std::string &_name) : size(_size), index(_index), name(_name) {}

uint64_t Type::addType(const uint64_t _size, const std::string &_name) {
    Type::globalTypes.push_back(Type(_size, Type::globalTypes.size(), _name));
    Type::typeMapping[_name] = Type::globalTypes.size() - 1;

    return Type::globalTypes.size() - 1;
}


uint64_t Type::findType(const std::string &name) {
    auto ptrToType = Type::typeMapping.find(name);
    if(ptrToType == Type::typeMapping.end()) {
        // Not in map
        TypeCheckingError("Type not found: ", name);
        return -1;
    } else {
        return ptrToType->second;
    }
}

std::vector<Type> Type::globalTypes = {
    Type(1, 0,  "bool"), 
    Type(8, 1, "int64"), 
    Type(0, 2,  "void"), 
};

std::map<std::string, uint64_t> Type::typeMapping = {
    { "bool", 0},
    {"int64", 1},
    { "void", 2}
};


void LiteralExpression::deduceType(Context &ctx) {
    if(this->type != -1) {return;}
    
    switch(this->value.type) {  
    // TODO: optimize
    case Lexing::TokenType::NUMBER: {
        this->type = TypeIndexes::INT64;
        break;
    }
    case Lexing::TokenType::BOOLEAN: {
        this->type = TypeIndexes::BOOL;
        break;
    }
    case Lexing::TokenType::NAME: {
        auto varIndex = ctx.findVariable(this->value.lexeme);
        this->type = ctx.variables[varIndex].type;
        break;
    }
    default:{
        TypeCheckingError("Unsupported type of literal: ", this->value.type);
    }
    }
}

void BinaryExpression::deduceType(Context &ctx) {
    if(this->type != -1) {return;}

    this->left->deduceType(ctx);
    this->right->deduceType(ctx);

    if(this->left->type != this->right->type) {
        // TODO: we want operator overloading
        TypeCheckingError("Types are incompatible : ", Type::globalTypes[this->left->type].name, " ", Type::globalTypes[this->right->type].name, ": for operation ", Lexing::TokenTypeName[this->operation]);
    }

    this->type = this->left->type;

    // This is a simple flag for making the code more readable
    bool isGoodOperation = false;

    switch(this->operation) {
    case Lexing::TokenType::PLUS: {
        isGoodOperation = this->type == TypeIndexes::INT64;
        break;
    }
    case Lexing::TokenType::MINUS: {
        isGoodOperation = this->type == TypeIndexes::INT64;
        break;
    }
    case Lexing::TokenType::STAR: {
        isGoodOperation = this->type == TypeIndexes::INT64;
        break;
    }
    case Lexing::TokenType::SLASH: {
        isGoodOperation = this->type == TypeIndexes::INT64;
        break;
    }
    case Lexing::TokenType::MODULO: {
        isGoodOperation = this->type == TypeIndexes::INT64;
        break;
    }
    case Lexing::TokenType::OR: {
        switch(this->type) {
            case Grammar::TypeIndexes::INT64: isGoodOperation = true; break;
            case Grammar::TypeIndexes::BOOL: isGoodOperation = true; break;
        }
        break;
    }
    case Lexing::TokenType::OROR: {
        switch(this->type) {
            case Grammar::TypeIndexes::INT64: isGoodOperation = true; break;
            case Grammar::TypeIndexes::BOOL: isGoodOperation = true; break;
        }
        break;
    }
    case Lexing::TokenType::AND: {
        switch(this->type) {
            case Grammar::TypeIndexes::INT64: isGoodOperation = true; break;
            case Grammar::TypeIndexes::BOOL: isGoodOperation = true; break;
        }
        break;
    }
    case Lexing::TokenType::ANDAND: {
        switch(this->type) {
            case Grammar::TypeIndexes::INT64: isGoodOperation = true; break;
            case Grammar::TypeIndexes::BOOL: isGoodOperation = true; break;
        }
        break;
    }
    case Lexing::TokenType::XOR: {
        switch(this->type) {
            case Grammar::TypeIndexes::INT64: isGoodOperation = true; break;
            case Grammar::TypeIndexes::BOOL: isGoodOperation = true; break;
        }
        break;
    }
    case Lexing::TokenType::XORXOR: {
        switch(this->type) {
            case Grammar::TypeIndexes::INT64: isGoodOperation = true; break;
            case Grammar::TypeIndexes::BOOL: isGoodOperation = true; break;
        }
        break;
    }
    case Lexing::TokenType::NOT: {
        isGoodOperation = this->type == TypeIndexes::INT64;
        break;
    }
    case Lexing::TokenType::BANG: {
        isGoodOperation = this->type == TypeIndexes::BOOL;
        break;
    }
    case Lexing::TokenType::EQUAL_EQUAL: {
        switch(this->type) {
            case Grammar::TypeIndexes::INT64: isGoodOperation = true; break;
            case Grammar::TypeIndexes::BOOL: isGoodOperation = true; break;
        }
        break;
    }
    case Lexing::TokenType::BANG_EQUAL: {
        switch(this->type) {
            case Grammar::TypeIndexes::INT64: isGoodOperation = true; break;
            case Grammar::TypeIndexes::BOOL: isGoodOperation = true; break;
        }
        break;
    }
    case Lexing::TokenType::SMALLER: {
        isGoodOperation = this->type == TypeIndexes::INT64;
        break;
    }
    case Lexing::TokenType::SMALLER_EQUAL: {
        isGoodOperation = this->type == TypeIndexes::INT64;
        break;
    }
    case Lexing::TokenType::BIGGER: {
        isGoodOperation = this->type == TypeIndexes::INT64;
        break;
    }
    case Lexing::TokenType::BIGGER_EQUAL: {
        isGoodOperation = this->type == TypeIndexes::INT64;
        break;
    }
    }

    if(!isGoodOperation) {
        TypeCheckingError("Types are not compatible for operation: ", Lexing::TokenTypeName[this->operation]);
    }
}

void UnaryExpression::deduceType(Context &ctx) {
    if(this->type != -1) {return;}

    this->expr->deduceType(ctx);

    this->type = this->expr->type;

    bool isGoodOperation = false;

    switch(this->operation) {
    case Lexing::TokenType::UNARY_PLUS: {
        isGoodOperation = this->type == TypeIndexes::INT64;
        break;
    }
    case Lexing::TokenType::UNARY_MINUS: {
        isGoodOperation = this->type == TypeIndexes::INT64;
        break;
    }
    case Lexing::TokenType::NOT: {
        isGoodOperation = this->type == TypeIndexes::INT64;
        break;
    }
    case Lexing::TokenType::BANG: {
        isGoodOperation = this->type == TypeIndexes::BOOL;
        break;
    }
    default: {
        TypeCheckingError("Unary operation ", Lexing::TokenTypeName[this->operation], " is currently not supported");
    }
    }

    if(!isGoodOperation) {
        TypeCheckingError("Types are not compatible for operation: ", Lexing::TokenTypeName[this->operation]);
    }
}

void FunctionCall::deduceType(Context &ctx) {
    if(this->type != -1) {return;}

    // TODO handle functions better

    if(this->name == "print") {
        this->type = TypeIndexes::VOID;
    } else if(this->name == "int64") {

        if(this->parameters.size() != 1) {
            TypeCheckingError("int64 cast was given wrong number of parameters: ", this->parameters.size());
        }
        this->parameters[0]->deduceType(ctx);

        if(this->parameters[0]->type != TypeIndexes::BOOL) {
            TypeCheckingError("Currently not supporting overloading");
        }

        this->type = TypeIndexes::INT64;        
    } else if(this->name == "bool") {

        if(this->parameters.size() != 1) {
            TypeCheckingError("bool cast was given wrong number of parameters: ", this->parameters.size());
        }
        this->parameters[0]->deduceType(ctx);

        if(this->parameters[0]->type != TypeIndexes::INT64) {
            TypeCheckingError("Currently not supporting overloading");
        }

        this->type = TypeIndexes::BOOL;        
    } else {
        TypeCheckingError("Function call type checking failed: only intrinsic functions are currently handled");
    }
}

};