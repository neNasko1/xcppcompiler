#include "Lexer.h"
#include "Grammar.h"
#include "Parser.h"

std::ostream& operator <<(std::ostream &os, const Statement &stmt) {
    return stmt.hiddenPrint(os);
}
Statement::Statement() {}
Statement::~Statement() {}

Expression::Expression() {}
Expression::~Expression() {}

LiteralExpression::LiteralExpression(const Token &_value) : value(_value) {}
LiteralExpression::~LiteralExpression() {}

std::ostream& LiteralExpression::hiddenPrint(std::ostream &os) const {
    os << Parser::getTabIdentation();
    os << this->value.rawValue;
    return os;
}

BinaryExpression::BinaryExpression(Expression *_left, const TokenType &_operation, Expression *_right) : left(_left), operation(_operation), right(_right) {}
BinaryExpression::~BinaryExpression() {delete this->left; delete this->right;}

std::ostream& BinaryExpression::hiddenPrint(std::ostream &os) const {
    os << Parser::getTabIdentation();
    os << "Binary expression {" << std::endl;
    Parser::addTabIdentation(+1);
        os << *(this->left) << std::endl;

        os << Parser::getTabIdentation();
        os << TokenTypeName[this->operation] << std::endl;

        os << *(this->right) << std::endl;
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
    Parser::addTabIdentation(+1);
        os << Parser::getTabIdentation();
        os << TokenTypeName[this->operation] << std::endl;

        os << *(this->expr) << std::endl;
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
    Parser::addTabIdentation(+1);
    for(const auto &param : this->parameters) {
        os << *(param) << std::endl;
    }
    Parser::addTabIdentation(-1);
    os << Parser::getTabIdentation();
    os << "}";
    return os;    
}

ExpressionStatement::ExpressionStatement(Expression *_expr) : expr(_expr) {}
ExpressionStatement::~ExpressionStatement() {delete expr;}
std::ostream& ExpressionStatement::hiddenPrint(std::ostream &os) const {
    os << Parser::getTabIdentation();
    os << "Expression statement { " << std::endl;
    Parser::addTabIdentation(+1);
        os << *(this->expr) << std::endl;
    Parser::addTabIdentation(-1);
    os << Parser::getTabIdentation();
    os << "}";
    return os;
}

