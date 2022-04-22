#pragma once

#include <iostream>

#include "../Lexer.h"
#include "Expression.h"

namespace Grammar {

class BinaryExpression final : public Expression {
private:
    std::ostream& hiddenPrint(std::ostream &os) const;

public:
    Expression *left;
    Lexing::TokenType operation;
    Expression *right;

	BinaryExpression(Expression *_left, const Lexing::TokenType &_operation, Expression *_right);
    ~BinaryExpression();
};

}

