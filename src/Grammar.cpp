#include "Lexer.h"
#include "Grammar.h"
#include "Parser.h"

Expression::Expression() {}
Expression::~Expression() {}

std::ostream& operator <<(std::ostream &os, const Expression &expr) {
    return expr.hiddenPrint(os);
}

LiteralExpression::LiteralExpression(const Token &_value) : value(_value) {}
LiteralExpression::~LiteralExpression() {}

std::ostream& LiteralExpression::hiddenPrint(std::ostream &os) const {
    // Print contents 
    os << Parser::getTabIdentation();
    os << this->value.rawValue;
    return os;
}

BinaryExpression::BinaryExpression(Expression *_left, const TokenType &_operation, Expression *_right) : left(_left), operation(_operation), right(_right) {}
BinaryExpression::~BinaryExpression() {delete this->left; delete this->right;}

std::ostream& BinaryExpression::hiddenPrint(std::ostream &os) const {
    os << Parser::getTabIdentation();
    os << "Binary expression {" << std::endl;
    // Make identation one tab deeper
    Parser::addTabIdentation(+1);

    // Print contents of expression
    os << *(this->left) << std::endl;

    os << Parser::getTabIdentation();
    os << TokenTypeName[this->operation] << std::endl;

    os << *(this->right) << std::endl;

    // Return identation to original level
    Parser::addTabIdentation(-1);
    os << Parser::getTabIdentation();
    os << "}";
    return os;
}

UnaryExpression::UnaryExpression(const TokenType &_operation, Expression *_expr) : operation(_operation), expr(_expr) {}
UnaryExpression::~UnaryExpression() {delete this->expr;}

std::ostream& UnaryExpression::hiddenPrint(std::ostream &os) const {
    os << Parser::getTabIdentation();
    os << "Unary expression {" << std::endl;
    // Make identation one tab deeper
    Parser::addTabIdentation(+1);

    // Print contents of expression
    os << Parser::getTabIdentation();
    os << TokenTypeName[this->operation] << std::endl;

    os << *(this->expr) << std::endl;

    // Return identation to original level
    Parser::addTabIdentation(-1);
    os << Parser::getTabIdentation();
    os << "}";
    return os;
}


FunctionCall::FunctionCall(std::string _name, std::vector<Expression*> &_parameters) : name(_name), parameters(_parameters) {} 
FunctionCall::~FunctionCall() {}

std::ostream &FunctionCall::hiddenPrint(std::ostream &os) const {
    os << Parser::getTabIdentation();
    os << "Function call " << this->name << " {" << std::endl;
    // Make identation one tab deeper
    Parser::addTabIdentation(+1);

    // Print all parameters
    for(const auto &param : this->parameters) {
        os << *(param) << std::endl;
    }

    // Return identation to original level
    Parser::addTabIdentation(-1);
    os << Parser::getTabIdentation();
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
ExpressionStatement::~ExpressionStatement() {delete expr;}
std::ostream& ExpressionStatement::hiddenPrint(std::ostream &os) const {
    os << Parser::getTabIdentation();
    os << "Expression statement { " << std::endl;
    // Make identation one tab deeper
    Parser::addTabIdentation(+1);

    // Print recursively the whole expression statement
    os << *(this->expr) << std::endl;

    // Return identation to original level
    Parser::addTabIdentation(-1);
    os << Parser::getTabIdentation();
    os << "}";
    return os;
}

StatementList::StatementList(const std::vector<Statement*> &_list) : list(_list) {}
StatementList::~StatementList() {}

std::ostream &StatementList::hiddenPrint(std::ostream &os) const {
    os << Parser::getTabIdentation();
    os << "Statement list { " << std::endl;
    // Make identation one tab deeper
    Parser::addTabIdentation(+1);

    // Print recursively the whole statement list
    for(const auto &expr : this->list) {
        os << *(expr) << std::endl;
    }

    // Return identation to original level
    Parser::addTabIdentation(-1);
    os << Parser::getTabIdentation();
    os << "}";
    return os;    
}

