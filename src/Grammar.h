#pragma once
#ifndef GRAMMAR_H
#define GRAMMAR_H

#include "Lexer.h"
#include "Parser.h"

class Statement {
private:
    virtual std::ostream& hiddenPrint(std::ostream &os) const = 0;
public:
    Statement();
    virtual ~Statement() = 0;
    friend std::ostream& operator <<(std::ostream &os, const Statement &expr);
};

class Expression : public Statement {
private:
    virtual std::ostream& hiddenPrint(std::ostream &os) const = 0;
public:
    Expression();
    virtual ~Expression() = 0;
};

class LiteralExpression final : public Expression {
private:
    std::ostream& hiddenPrint(std::ostream &os) const;
public:
    Token value;
    LiteralExpression(const Token &_value);
    ~LiteralExpression();
};

class BinaryExpression final : public Expression {
private:
    std::ostream& hiddenPrint(std::ostream &os) const;
public:
    Expression *left;
    TokenType operation;
    Expression *right;
    BinaryExpression(Expression *_left, const TokenType &_operation, Expression *_right);
    ~BinaryExpression();
};

class UnaryExpression final : public Expression {
private:
    std::ostream& hiddenPrint(std::ostream &os) const;
public:
    TokenType operation;
    Expression *expr;
    UnaryExpression(const TokenType &_operation, Expression *_expr);
    ~UnaryExpression();
};

class ExpressionStatement final : public Statement {
private:
    std::ostream& hiddenPrint(std::ostream &os) const;
public:
    Expression *expr;
    ExpressionStatement(Expression *_expr);
    ~ExpressionStatement();
};

#endif // GRAMMAR_H
