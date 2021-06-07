#pragma once
#ifndef GRAMMAR_H
#define GRAMMAR_H

#include "Lexer.h"

class Expression {
private:
    virtual std::ostream& hiddenPrint(std::ostream &os) const = 0;
public:
    Expression();
    virtual ~Expression() = 0;
    friend std::ostream& operator <<(std::ostream &os, const Expression &expr);
};

class LiteralExpression final : public Expression {
private:
    std::ostream& hiddenPrint(std::ostream &os) const;
public:
    Token value;
    LiteralExpression(const Token &_value);
    ~LiteralExpression();
};

class BinaryExpression final : public Expression{
private:
    std::ostream& hiddenPrint(std::ostream &os) const;
public:
    Expression *left;
    Token operation;
    Expression *right;
    BinaryExpression(Expression *_left, const Token &_operation, Expression *_right);
    ~BinaryExpression();
};

class UnaryExpression final : public Expression {
private:
    std::ostream& hiddenPrint(std::ostream &os) const;
public:
    Token operation;
    Expression *expr;
    UnaryExpression(const Token &_operation, Expression *_expr);
    ~UnaryExpression();
};

class GroupedExpression final : public Expression {
private:
    std::ostream& hiddenPrint(std::ostream &os) const;
public:
    Expression *expr;
    Token bracket;
    GroupedExpression(Expression *_expr, const Token _bracket = Token(TokenType::L_PAREN));
    ~GroupedExpression();
};

#endif // GRAMMAR_H
