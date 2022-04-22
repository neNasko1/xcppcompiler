#include <iostream>

#include "../Lexer.h"
#include "../Grammar.h"
#include "../Parser.h"

namespace Grammar {

IfStatement::IfStatement(Expression *condition, Statement *ifBody, Statement *elseBody)
        : condition(condition), ifBody(ifBody), elseBody(elseBody) {}

IfStatement::~IfStatement() {
    delete this->condition;
    delete this->ifBody;
    delete this->elseBody;
}

std::ostream &IfStatement::hiddenPrint(std::ostream &os) const {
    os << Parsing::Parser::getTabIdentation();
    os << "If statement { " << std::endl;

    os << Parsing::Parser::getTabIdentation();
    os << ">Condition :" << std::endl;
    Parsing::Parser::addTabIdentation(+1);
    os << *(this->condition) << std::endl;
    Parsing::Parser::addTabIdentation(-1);

    os << Parsing::Parser::getTabIdentation();
    os << ">If-body :" << std::endl;
    Parsing::Parser::addTabIdentation(+1);
    os << *(this->ifBody) << std::endl;
    Parsing::Parser::addTabIdentation(-1);

    // Else if not mandatory
    if(this->elseBody) {
        os << Parsing::Parser::getTabIdentation();
        os << ">Else body :" << std::endl;
        Parsing::Parser::addTabIdentation(+1);
        os << *(this->elseBody) << std::endl;
        Parsing::Parser::addTabIdentation(-1);
    }

    os << Parsing::Parser::getTabIdentation();
    os << "}";
    return os;
}

};
