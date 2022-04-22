#include <iostream>

#include "../Lexer.h"
#include "../Grammar.h"
#include "../Parser.h"

namespace Grammar {

ExpressionStatement::ExpressionStatement(Expression *_expr) : expr(_expr) {}

ExpressionStatement::~ExpressionStatement() {
    delete this->expr;
}

std::ostream& ExpressionStatement::hiddenPrint(std::ostream &os) const {
    os << Parsing::Parser::getTabIdentation();
    os << "Expression statement { " << std::endl;
    // Make identation one tab deeper
    Parsing::Parser::addTabIdentation(+1);

    // Print recursively the whole expression statement
    os << *(this->expr) << std::endl;

    // Return identation to original level
    Parsing::Parser::addTabIdentation(-1);
    os << Parsing::Parser::getTabIdentation();
    os << "}";
    return os;
}

};
