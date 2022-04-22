#pragma once

#include <vector>
#include <iostream>

#include "Expression.h"

namespace Grammar {

class Expression {
private:
    virtual std::ostream& hiddenPrint(std::ostream &os) const = 0;

public:

	Expression();
    virtual ~Expression() = 0;
    friend std::ostream& operator <<(std::ostream &os, const Expression &expr);
};

}
