#include <iostream>

#include "../Lexer.h"
#include "../Grammar.h"
#include "../Parser.h"

namespace Grammar {

LiteralExpression::LiteralExpression(const Lexing::Token &_value) : value(_value) {}

LiteralExpression::~LiteralExpression() {}

std::ostream& LiteralExpression::hiddenPrint(std::ostream &os) const {
    // Print contents
    os << Parsing::Parser::getTabIdentation();
    os << this->value.lexeme;
    return os;
}

};
