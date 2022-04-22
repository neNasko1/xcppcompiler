#pragma once

#include <iostream>
#include <vector>
#include <string>

#include "../Lexer.h"

namespace Grammar {

class FunctionCall final : public Expression {
private:
    std::ostream& hiddenPrint(std::ostream &os) const;

public:
    std::string name;
    std::vector<Expression*> parameters;

    FunctionCall(std::string _name, std::vector<Expression*> &_parameters);
    ~FunctionCall();
};

};
