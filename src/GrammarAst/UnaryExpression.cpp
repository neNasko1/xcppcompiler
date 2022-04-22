#include <iostream>

#include "../Lexer.h"
#include "../Grammar.h"
#include "../Parser.h"

namespace Grammar {

UnaryExpression::UnaryExpression(const Lexing::TokenType &_operation, Expression *_expr) : operation(_operation), expr(_expr) {}

UnaryExpression::~UnaryExpression() {
    delete this->expr;
}

std::ostream& UnaryExpression::hiddenPrint(std::ostream &os) const {
    os << Parsing::Parser::getTabIdentation();
    os << "Unary expression {" << std::endl;
    // Make identation one tab deeper
    Parsing::Parser::addTabIdentation(+1);

    // Print contents of expression
    os << Parsing::Parser::getTabIdentation();
    os << Lexing::TokenTypeName[this->operation] << std::endl;

    os << *(this->expr) << std::endl;

    // Return identation to original level
    Parsing::Parser::addTabIdentation(-1);
    os << Parsing::Parser::getTabIdentation();
    os << "}";
    return os;
}

};
