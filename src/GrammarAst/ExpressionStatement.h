#pragma once

#include <iostream>

#include "Expression.h"

namespace Grammar {

class ExpressionStatement final : public Statement {
private:
    std::ostream& hiddenPrint(std::ostream &os) const;
public:
	Expression *expr;

	ExpressionStatement(Expression *_expr);
    ~ExpressionStatement();
};

}
