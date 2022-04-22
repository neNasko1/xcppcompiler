#include <iostream>

#include "../Lexer.h"
#include "../Grammar.h"
#include "../Parser.h"

namespace Grammar {

Statement::Statement() {}

Statement::~Statement() {}
// Overloaded operator << for printing statement to ostream

std::ostream& operator <<(std::ostream &os, const Statement &stmt) {
    return stmt.hiddenPrint(os);
}

};
