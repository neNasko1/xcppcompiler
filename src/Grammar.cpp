#include "Lexer.h"
#include "Grammar.h"

Expression::Expression() {}
Expression::~Expression() {}

std::ostream& operator <<(std::ostream &os, const Expression &expr) {
    return expr.hiddenPrint(os);
}

LiteralExpression::LiteralExpression(const Token &_value) : value(_value) {}
LiteralExpression::~LiteralExpression() {}

std::ostream& LiteralExpression::hiddenPrint(std::ostream &os) const {
    return os << this->value.rawValue;
}

BinaryExpression::BinaryExpression(Expression *_left, const Token &_operation, Expression *_right) : left(_left), operation(_operation), right(_right) {}
BinaryExpression::~BinaryExpression() {delete this->left; delete this->right;}

std::ostream& BinaryExpression::hiddenPrint(std::ostream &os) const {
    if(!this->left || !this->right) {
        return os << "Bad expression ";
    } else {
        return os << *(this->left) << " " << TokenTypeName[this->operation.type] << " " << *(this->right);
    }
}

UnaryExpression::UnaryExpression(const Token &_operation, Expression *_expr) : operation(_operation), expr(_expr) {}
UnaryExpression::~UnaryExpression() {delete this->expr;}

std::ostream& UnaryExpression::hiddenPrint(std::ostream &os) const {
    if(!this->expr) {
        return os << "Bad expression ";
    } else {
        return os << TokenTypeName[this->operation.type] << *(this->expr);
    }
}

GroupedExpression::GroupedExpression(Expression *_expr, const Token _bracket) : expr(_expr), bracket(_bracket) {}
GroupedExpression::~GroupedExpression() {delete this->expr;}

std::ostream& GroupedExpression::hiddenPrint(std::ostream &os) const {
    if(!this->expr) {
        return os << "Bad expression ";
    } else {
        return os << "(" << *(this->expr) << ")";
    }
}
