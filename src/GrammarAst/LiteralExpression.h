#pragma once

#include <iostream>

#include "../Lexer.h"
#include "Expression.h"

namespace Grammar {

class LiteralExpression final : public Expression {
private:
    std::ostream& hiddenPrint(std::ostream &os) const;

public:
    Lexing::Token value;

	LiteralExpression(const Lexing::Token &_value);
    ~LiteralExpression();
};

}
