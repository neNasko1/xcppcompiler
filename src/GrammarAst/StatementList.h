#pragma once

#include <iostream>
#include <vector>

#include "../Lexer.h"
#include "Statement.h"

namespace Grammar {

class StatementList final : public Statement {
private:
    std::ostream& hiddenPrint(std::ostream &os) const;
public:
    std::vector<Statement*> list;

	StatementList(const std::vector<Statement*> &_list = {});
    ~StatementList();
};

};

