#include <iostream>

#include "../Lexer.h"
#include "../Grammar.h"
#include "../Parser.h"

namespace Grammar {

DeclarationStatement::DeclarationStatement(const std::string &_name, const std::string &_type, Expression *_expr) : name(_name), type(_type), expr(_expr) {}

DeclarationStatement::~DeclarationStatement() {
    delete this->expr;
}

std::ostream& DeclarationStatement::hiddenPrint(std::ostream &os) const {
    os << Parsing::Parser::getTabIdentation();
    os << "Declaration statement { " << std::endl;

    // Make identation one tab deeper
    Parsing::Parser::addTabIdentation(+1);

    os << Parsing::Parser::getTabIdentation();
    os << this->name << " : " << this->type << std::endl;

    if(this->expr) {
        // Print recursively the whole expression statement
        os << *(this->expr) << std::endl;
    }

    // Return identation to original level
    Parsing::Parser::addTabIdentation(-1);

    os << Parsing::Parser::getTabIdentation();
    os << "}";
    return os;
}

};
