#pragma once

#include <iostream>

#include "Statement.h"
#include "Expression.h"

namespace Grammar {

class IfStatement final : public Statement {
private:
    std::ostream& hiddenPrint(std::ostream &os) const;

public:
    Expression *condition;
    Statement *ifBody;
    Statement *elseBody;

    IfStatement(Expression *condition, Statement *ifBody, Statement *elseBody);
    ~IfStatement();
};

};
