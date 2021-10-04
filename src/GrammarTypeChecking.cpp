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
    std::cerr << "There was an error while VM code was being generated " << "\n";
    TypeCheckingErrorPrint(t...);
    exit(0);
}

Type::~Type() {}
Type::Type(const uint64_t _size, const int32_t _index, const std::string &_name) : size(_size), index(_index), name(_name) {}

Type* Type::addType(const uint64_t _size, const std::string &_name) {
    Type::globalTypes.push_back(Type(_size, Type::globalTypes.size(), _name));
    return &(globalTypes.back());
}


Type* Type::findType(const std::string &name) {
    auto ptrToType = Type::typeMapping.find(name);
    if(ptrToType == Type::typeMapping.end()) {
        // Not in map
        TypeCheckingError("Type not found: ", name);
        return nullptr;
    } else {
        return ptrToType->second;
    }
}

std::vector<Type> Type::globalTypes = {
    Type(1, 0,  "bool"), 
    Type(8, 1, "int64"), 
    Type(0, 2,  "void"), 
};

std::map<std::string, Type*> Type::typeMapping = {
    { "bool", &(Type::globalTypes[0])},
    {"int64", &(Type::globalTypes[1])},
    { "void", &(Type::globalTypes[2])}
};


void LiteralExpression::deduceType() {
    if(this->type != nullptr) {return;}
    
    switch(this->value.type) {  
    // TODO: optimize
    case Lexing::TokenType::NUMBER: {
        this->type = Type::findType(std::string("int64"));
        break;
    }
    case Lexing::TokenType::BOOLEAN: {
        this->type = Type::findType(std::string("bool"));
        break;
    }
    default:{
        TypeCheckingError("Unsupported type of literal: ", this->value.type);
    }
    }
}

void BinaryExpression::deduceType() {
    if(this->type != nullptr) {return;}

    this->left->deduceType();
    this->right->deduceType();

    if(this->left->type->index != this->right->type->index) {
        // TODO: we want operator overloading
        TypeCheckingError("Types are incompatible : ", this->left->type->name, " ", this->right->type->name, ": for operation ", Lexing::TokenTypeName[this->operation]);
    }

    this->type = this->left->type;

    // This is a simple flag for making the code more readable
    bool goodOperation = false;

    switch(this->operation) {
    case Lexing::TokenType::PLUS: {
        goodOperation = this->type->index == TypeIndexes::INT64;
        break;
    }
    case Lexing::TokenType::MINUS: {
        goodOperation = this->type->index == TypeIndexes::INT64;
        break;
    }
    case Lexing::TokenType::STAR: {
        goodOperation = this->type->index == TypeIndexes::INT64;
        break;
    }
    case Lexing::TokenType::SLASH: {
        goodOperation = this->type->index == TypeIndexes::INT64;
        break;
    }
    case Lexing::TokenType::MODULO: {
        goodOperation = this->type->index == TypeIndexes::INT64;
        break;
    }
    case Lexing::TokenType::OR: {
        switch(this->type->index) {
            case Grammar::TypeIndexes::INT64: goodOperation = true; break;
            case Grammar::TypeIndexes::BOOL: goodOperation = true; break;
        }
        break;
    }
    case Lexing::TokenType::OROR: {
        switch(this->type->index) {
            case Grammar::TypeIndexes::INT64: goodOperation = true; break;
            case Grammar::TypeIndexes::BOOL: goodOperation = true; break;
        }
        break;
    }
    case Lexing::TokenType::AND: {
        switch(this->type->index) {
            case Grammar::TypeIndexes::INT64: goodOperation = true; break;
            case Grammar::TypeIndexes::BOOL: goodOperation = true; break;
        }
        break;
    }
    case Lexing::TokenType::ANDAND: {
        switch(this->type->index) {
            case Grammar::TypeIndexes::INT64: goodOperation = true; break;
            case Grammar::TypeIndexes::BOOL: goodOperation = true; break;
        }
        break;
    }
    case Lexing::TokenType::XOR: {
        switch(this->type->index) {
            case Grammar::TypeIndexes::INT64: goodOperation = true; break;
            case Grammar::TypeIndexes::BOOL: goodOperation = true; break;
        }
        break;
    }
    case Lexing::TokenType::XORXOR: {
        switch(this->type->index) {
            case Grammar::TypeIndexes::INT64: goodOperation = true; break;
            case Grammar::TypeIndexes::BOOL: goodOperation = true; break;
        }
        break;
    }
    case Lexing::TokenType::NOT: {
        goodOperation = this->type->index == TypeIndexes::INT64;
        break;
    }
    case Lexing::TokenType::BANG: {
        goodOperation = this->type->index == TypeIndexes::BOOL;
        break;
    }
    case Lexing::TokenType::EQUAL_EQUAL: {
        switch(this->type->index) {
            case Grammar::TypeIndexes::INT64: goodOperation = true; break;
            case Grammar::TypeIndexes::BOOL: goodOperation = true; break;
        }
        break;
    }
    case Lexing::TokenType::BANG_EQUAL: {
        switch(this->type->index) {
            case Grammar::TypeIndexes::INT64: goodOperation = true; break;
            case Grammar::TypeIndexes::BOOL: goodOperation = true; break;
        }
        break;
    }
    case Lexing::TokenType::SMALLER: {
        goodOperation = this->type->index == TypeIndexes::INT64;
        break;
    }
    case Lexing::TokenType::SMALLER_EQUAL: {
        goodOperation = this->type->index == TypeIndexes::INT64;
        break;
    }
    case Lexing::TokenType::BIGGER: {
        goodOperation = this->type->index == TypeIndexes::INT64;
        break;
    }
    case Lexing::TokenType::BIGGER_EQUAL: {
        goodOperation = this->type->index == TypeIndexes::INT64;
        break;
    }
    }

    if(!goodOperation) {
        TypeCheckingError("Types are not compatible for operation: ", Lexing::TokenTypeName[this->operation]);
    }
}

void UnaryExpression::deduceType() {
    if(this->type != nullptr) {return;}

    this->expr->deduceType();

    this->type = this->expr->type;
}

void FunctionCall::deduceType() {
    if(this->type != nullptr) {return;}

    // TODO handle functions better

    if(this->name == "print") {
        this->type = &(Type::globalTypes[TypeIndexes::VOID]);
    } else if(this->name == "int64") {

        if(this->parameters.size() != 1) {
            TypeCheckingError("int64 cast was given wrong number of parameters: ", this->parameters.size());
        }
        this->parameters[0]->deduceType();

        if(this->parameters[0]->type->index != TypeIndexes::BOOL) {
            TypeCheckingError("Currently not supporting overloading");
        }

        this->type = &(Type::globalTypes[TypeIndexes::INT64]);        
    } else if(this->name == "bool") {

        if(this->parameters.size() != 1) {
            TypeCheckingError("bool cast was given wrong number of parameters: ", this->parameters.size());
        }
        this->parameters[0]->deduceType();

        if(this->parameters[0]->type->index != TypeIndexes::INT64) {
            TypeCheckingError("Currently not supporting overloading");
        }

        this->type = &(Type::globalTypes[TypeIndexes::BOOL]);        
    } else {
        GrammarBytecodeError("Function call type checking failed: only intrinsic functions are currently handled");
    }
}

};