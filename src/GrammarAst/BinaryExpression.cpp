#include <iostream>

#include "../Parser.h"
#include "../Lexer.h"
#include "Expression.h"

namespace Grammar {

BinaryExpression::BinaryExpression(Expression *_left, const Lexing::TokenType &_operation, Expression *_right)
	: left(_left), operation(_operation), right(_right) {}

BinaryExpression::~BinaryExpression() {
    delete this->left;
    delete this->right;
}

std::ostream& BinaryExpression::hiddenPrint(std::ostream &os) const {
    os << Parsing::Parser::getTabIdentation();
    os << "Binary expression {" << std::endl;
    // Make identation one tab deeper
    Parsing::Parser::addTabIdentation(+1);

    // Print contents of expression
    os << *(this->left) << std::endl;

    os << Parsing::Parser::getTabIdentation();
    os << Lexing::TokenTypeName[this->operation] << std::endl;

    os << *(this->right) << std::endl;

    // Return identation to original level
    Parsing::Parser::addTabIdentation(-1);
    os << Parsing::Parser::getTabIdentation();
    os << "}";
    return os;
}

};
