#pragma once
#ifndef GRAMMAR_H
#define GRAMMAR_H

#include <vector>

#include "Lexer.h"

/**
 * @brief Abstract class implementing the expression language construct
 * 
 */
class Expression {
private:
    /**
     * @brief Print contents of an expression recursively to std::ostream
     * 
     * @param os Stream to output to
     * @return std::ostream&  Modified stream
     */
    virtual std::ostream& hiddenPrint(std::ostream &os) const = 0;

public:
    /**
     * @brief Construct a new Expression object
     * 
     */
    Expression();

    /**
     * @brief Destroy the Expression object
     * 
     */
    virtual ~Expression() = 0;

    /**
     * @brief Friend function outputing contents of an expression to std::ostream
     * 
     * @param os ostream to print to
     * @param expr expression to output
     * @return std::ostream& modified ostream
     */
    friend std::ostream& operator <<(std::ostream &os, const Expression &expr);
};

/**
 * @brief Class representing literal expression, e.g. 10, 'c', 1.2
 * 
 */
class LiteralExpression final : public Expression {
private:
    /**
     * @brief Print contents of an expression recursively to std::ostream
     * 
     * @param os Stream to output to
     * @return std::ostream&  Modified stream
     */
    std::ostream& hiddenPrint(std::ostream &os) const;

public:
    /**
     * @brief Token which contains information about the type of literal;
     * 
     */
    Token value;

    /**
     * @brief Construct a new Literal Expression object
     *  
     * @param _value Token value containing information about the literal
     */
    LiteralExpression(const Token &_value);

    /**
     * @brief Destroy the Literal Expression object
     * 
     */
    ~LiteralExpression();
};

/**
 * @brief Class representing binary expression, e.g. exp1 + exp2
 * 
 */
class BinaryExpression final : public Expression {
private:
    /**
     * @brief Print contents of an expression recursively to std::ostream
     * 
     * @param os Stream to output to
     * @return std::ostream&  Modified stream
     */
    std::ostream& hiddenPrint(std::ostream &os) const;
public:
    /**
     * @brief Left operand in binary expression
     * 
     */
    Expression *left;

    /**
     * @brief Type of binary operation
     * 
     */
    TokenType operation;

    /**
     * @brief Right operand in binary expression
     * 
     */
    Expression *right;

    /**
     * @brief Construct a new Binary Expression object
     * 
     * @param _left Left operand of the binary expression
     * @param _operation Type of operation of the binary expression
     * @param _right Right operand of the binary expression
     */
    BinaryExpression(Expression *_left, const TokenType &_operation, Expression *_right);

    /**
     * @brief Destroy the Binary Expression object
     * 
     */
    ~BinaryExpression();
};

/**
 * @brief Class respresenting unary operation, e.g. -10, *a
 * 
 */
class UnaryExpression final : public Expression {
private:
    /**
     * @brief Print contents of an expression recursively to std::ostream
     * 
     * @param os Stream to output to
     * @return std::ostream&  Modified stream
     */
    std::ostream& hiddenPrint(std::ostream &os) const;
public:
    /**
     * @brief Type of unary operation
     * 
     */
    TokenType operation;

    /**
     * @brief Sub-expression 
     * 
     */
    Expression *expr;

    /**
     * @brief Construct a new Unary Expression object
     * 
     * @param _operation Type of unary operation of the unary expression(should be unary)
     * @param _expr Sub-expression of the unary expression
     */
    UnaryExpression(const TokenType &_operation, Expression *_expr);

    /**
     * @brief Destroy the Unary Expression object
     * 
     */
    ~UnaryExpression();
};

/**
 * @brief Class representing function call
 * 
 */
class FunctionCall final : public Expression {
private:
    /**
     * @brief Print contents of an expression recursively to std::ostream
     * 
     * @param os Stream to output to
     * @return std::ostream&  Modified stream
     */
    std::ostream& hiddenPrint(std::ostream &os) const;
public:
    /**
     * @brief Name of function being called
     * 
     */
    std::string name;

    /**
     * @brief Vector containing all parameters with which the function is being called
     * 
     */
    std::vector<Expression*> parameters;

    /**
     * @brief Construct a new Function Call object
     * 
     * @param _name Name of the function being called
     * @param _parameters Parameters with which the function is being called
     */
    FunctionCall(std::string _name, std::vector<Expression*> &_parameters);

    /**
     * @brief Destroy the Function Call object
     * 
     */
    ~FunctionCall();
};

/**
 * @brief Abstract class implementing the statement language construct
 * 
 */
class Statement {
private:
    /**
     * @brief Print contents of a statement recursively to std::ostream
     * 
     * @param os Stream to output to
     * @return std::ostream&  Modified stream
     */
    virtual std::ostream& hiddenPrint(std::ostream &os) const = 0;
public:
    /**
     * @brief Construct a new Statement object
     * 
     */
    Statement();

    /**
     * @brief Destroy the Statement object
     * 
     */
    virtual ~Statement() = 0;
    
    friend std::ostream& operator <<(std::ostream &os, const Statement &expr);
};

/**
 * @brief Class representing a statement consisting only of an expression
 * 
 */
class ExpressionStatement final : public Statement {
private:
    /**
     * @brief Print contents of a statement recursively to std::ostream
     * 
     * @param os Stream to output to
     * @return std::ostream&  Modified stream
     */
    std::ostream& hiddenPrint(std::ostream &os) const;
public:
    /**
     * @brief Expression value of the statement 
     * 
     */
    Expression *expr;

    /**
     * @brief Construct a new Expression Statement object
     * 
     * @param _expr 
     */
    ExpressionStatement(Expression *_expr);

    /**
     * @brief Destroy the Expression Statement object
     * 
     */
    ~ExpressionStatement();
};

/**
 * @brief Class representing an if statement
 * 
 */
class IfStatement final : public Statement {
private:
    /**
     * @brief Print contents of a statement recursively to std::ostream
     * 
     * @param os Stream to output to
     * @return std::ostream&  Modified stream
     */
    std::ostream& hiddenPrint(std::ostream &os) const;
public:

    /**
     * @brief Condition to check before executing if-body or else-body
     * 
     */
    Expression *condition;

    /**
     * @brief If-body
     * 
     */
    Statement *ifBody; 

    /**
     * @brief Else-body
     * 
     */
    Statement *elseBody;
    //note this can be an if statement, representing elif chains
};

/**
 * @brief Class representing a list of statements
 * 
 */
class StatementList final : public Statement {
private:
    /**
     * @brief Print contents of a statement list recursively to std::ostream
     * 
     * @param os Stream to output to
     * @return std::ostream&  Modified stream
     */
    std::ostream& hiddenPrint(std::ostream &os) const;
public:
    std::vector<Statement*> list;

    /**
     * @brief Construct a new Statement List object
     * 
     */
    StatementList(const std::vector<Statement*> &_list = {});

    /**
     * @brief Destroy the Statement List object
     * 
     */
    ~StatementList();
};

#endif // GRAMMAR_H
