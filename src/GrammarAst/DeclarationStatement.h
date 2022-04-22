#pragma once

#include <iostream>
#include <string>

#include "Expression.h"

namespace Grammar {

class DeclarationStatement final : public Statement {
private:
    std::ostream& hiddenPrint(std::ostream &os) const;

public:
    std::string name;
    std::string type;
    Expression *expr;

    DeclarationStatement(const std::string &_name, const std::string &_type = "", Expression *_expr = nullptr);
    ~DeclarationStatement();
};

};
