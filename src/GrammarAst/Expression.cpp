#include <iostream>

#include "../Grammar.h"
#include "Expression.h"

namespace Grammar {

Expression::Expression() {}

Expression::~Expression() {}

std::ostream& operator <<(std::ostream &os, const Expression &expr) {
    return expr.hiddenPrint(os);
}

};
