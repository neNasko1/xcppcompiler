#pragma once

#include <iostream>

#include "../Lexer.h"
#include "Expression.h"

namespace Grammar {

class UnaryExpression final : public Expression {
private:
    std::ostream& hiddenPrint(std::ostream &os) const;

public:
    Lexing::TokenType operation;
	Expression *expr;

    UnaryExpression(const Lexing::TokenType &_operation, Expression *_expr);
    ~UnaryExpression();
};

}

