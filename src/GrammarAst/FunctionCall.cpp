#include <iostream>

#include "../Lexer.h"
#include "../Grammar.h"
#include "../Parser.h"

namespace Grammar {

FunctionCall::FunctionCall(std::string _name, std::vector<Expression*> &_parameters) : name(_name), parameters(_parameters) {}

FunctionCall::~FunctionCall() {
    for(auto it : this->parameters) {
        delete it;
    }
}

std::ostream &FunctionCall::hiddenPrint(std::ostream &os) const {
    os << Parsing::Parser::getTabIdentation();
    os << "Function call " << this->name << " {" << std::endl;
    // Make identation one tab deeper
    Parsing::Parser::addTabIdentation(+1);

    // Print all parameters
    for(const auto &param : this->parameters) {
        os << *(param) << std::endl;
    }

    // Return identation to original level
    Parsing::Parser::addTabIdentation(-1);
    os << Parsing::Parser::getTabIdentation();
    os << "}";
    return os;
}

};
