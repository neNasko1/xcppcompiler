#include <iostream>
#include <vector>

#include "../Lexer.h"
#include "../Grammar.h"
#include "../Parser.h"

namespace Grammar {

StatementList::StatementList(const std::vector<Statement*> &_list) : list(_list) {}

StatementList::~StatementList() {
    for(auto it : this->list) {
        delete it;
    }
}

std::ostream &StatementList::hiddenPrint(std::ostream &os) const {
    os << Parsing::Parser::getTabIdentation();
    os << "Statement list { " << std::endl;
    // Make identation one tab deeper
    Parsing::Parser::addTabIdentation(+1);

    // Print recursively the whole statement list
    for(const auto &expr : this->list) {
        os << *(expr) << std::endl;
    }

    // Return identation to original level
    Parsing::Parser::addTabIdentation(-1);
    os << Parsing::Parser::getTabIdentation();
    os << "}";
    return os;
}

};
