#include "Lexer.h"
#include "Grammar.h"
#include "Parser.h"

namespace Grammar {

Expression::Expression() {}
Expression::~Expression() {}

std::ostream& operator <<(std::ostream &os, const Expression &expr) {
    return expr.hiddenPrint(os);
}

LiteralExpression::LiteralExpression(const Lexing::Token &_value) : value(_value) {}
LiteralExpression::~LiteralExpression() {}

std::ostream& LiteralExpression::hiddenPrint(std::ostream &os) const {
    // Print contents 
    os << Parsing::Parser::getTabIdentation();
    os << this->value.lexeme;
    return os;
}

BinaryExpression::BinaryExpression(Expression *_left, const Lexing::TokenType &_operation, Expression *_right) : left(_left), operation(_operation), right(_right) {}
BinaryExpression::~BinaryExpression() {
    delete this->left; 
    delete this->right;
}

std::ostream& BinaryExpression::hiddenPrint(std::ostream &os) const {
    os << Parsing::Parser::getTabIdentation();
    os << "Binary expression {" << std::endl;
    // Make identation one tab deeper
    Parsing::Parser::addTabIdentation(+1);

    // Print contents of expression
    os << *(this->left) << std::endl;

    os << Parsing::Parser::getTabIdentation();
    os << Lexing::TokenTypeName[this->operation] << std::endl;

    os << *(this->right) << std::endl;

    // Return identation to original level
    Parsing::Parser::addTabIdentation(-1);
    os << Parsing::Parser::getTabIdentation();
    os << "}";
    return os;
}

UnaryExpression::UnaryExpression(const Lexing::TokenType &_operation, Expression *_expr) : operation(_operation), expr(_expr) {}
UnaryExpression::~UnaryExpression() {
    delete this->expr;
}

std::ostream& UnaryExpression::hiddenPrint(std::ostream &os) const {
    os << Parsing::Parser::getTabIdentation();
    os << "Unary expression {" << std::endl;
    // Make identation one tab deeper
    Parsing::Parser::addTabIdentation(+1);

    // Print contents of expression
    os << Parsing::Parser::getTabIdentation();
    os << Lexing::TokenTypeName[this->operation] << std::endl;

    os << *(this->expr) << std::endl;

    // Return identation to original level
    Parsing::Parser::addTabIdentation(-1);
    os << Parsing::Parser::getTabIdentation();
    os << "}";
    return os;
}


FunctionCall::FunctionCall(std::string _name, std::vector<Expression*> &_parameters) : name(_name), parameters(_parameters) {} 
FunctionCall::~FunctionCall() {
    for(auto it : this->parameters) {
        delete it;
    }
}

std::ostream &FunctionCall::hiddenPrint(std::ostream &os) const {
    os << Parsing::Parser::getTabIdentation();
    os << "Function call " << this->name << " {" << std::endl;
    // Make identation one tab deeper
    Parsing::Parser::addTabIdentation(+1);

    // Print all parameters
    for(const auto &param : this->parameters) {
        os << *(param) << std::endl;
    }

    // Return identation to original level
    Parsing::Parser::addTabIdentation(-1);
    os << Parsing::Parser::getTabIdentation();
    os << "}";
    return os;    
}

Statement::Statement() {}
Statement::~Statement() {}
// Overloaded operator << for printing statement to ostream 
std::ostream& operator <<(std::ostream &os, const Statement &stmt) {
    return stmt.hiddenPrint(os);
}

ExpressionStatement::ExpressionStatement(Expression *_expr) : expr(_expr) {}
ExpressionStatement::~ExpressionStatement() {
    delete this->expr;
}
std::ostream& ExpressionStatement::hiddenPrint(std::ostream &os) const {
    os << Parsing::Parser::getTabIdentation();
    os << "Expression statement { " << std::endl;
    // Make identation one tab deeper
    Parsing::Parser::addTabIdentation(+1);

    // Print recursively the whole expression statement
    os << *(this->expr) << std::endl;

    // Return identation to original level
    Parsing::Parser::addTabIdentation(-1);
    os << Parsing::Parser::getTabIdentation();
    os << "}";
    return os;
}

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