#pragma once
#ifndef GRAMMAR_H
#define GRAMMAR_H

#include <vector>

#include "Lexer.h"
#include "VirtualMachine.h"
#include "GrammarTypeChecking.h"
#include "GrammarBytecode.h"

namespace Grammar {

template <typename... T> 
void GrammarBytecodeError(T... t);

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
     * @brief Return value type
     * 
     */
    uint64_t type;

    /**
     * @brief Destroy the Expression object
     * 
     */
    virtual ~Expression() = 0;

    /**
     * @brief Generate virtual machine bytecode and write it to buffer
     * 
     * @param buffer 
     */
    virtual void generateBytecode(std::vector<VM::Byte> &buffer, Context &ctx) = 0;

    /**
     * @brief Deduce return type of expression
     * 
     */
    virtual void deduceType(Context &ctx) = 0;

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
     * @brief Lexing::Token which contains information about the type of literal;
     * 
     */
    Lexing::Token value;

    /**
     * @brief Construct a new Literal Expression object
     *  
     * @param _value Lexing::Token value containing information about the literal
     */
    LiteralExpression(const Lexing::Token &_value);

    /**
     * @brief Destroy the Literal Expression object
     * 
     */
    ~LiteralExpression();

    /**
     * @brief Generate virtual machine bytecode and write it to buffer
     * 
     * @param buffer 
     */
    void generateBytecode(std::vector<VM::Byte> &buffer, Context &ctx);

    /**
     * @brief Deduce return type of expression
     * 
     */
    void deduceType(Context &ctx);
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
    Lexing::TokenType operation;

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
    BinaryExpression(Expression *_left, const Lexing::TokenType &_operation, Expression *_right);

    /**
     * @brief Destroy the Binary Expression object
     * 
     */
    ~BinaryExpression();

    /**
     * @brief Generate virtual machine bytecode and write it to buffer
     * 
     * @param buffer 
     */
    void generateBytecode(std::vector<VM::Byte> &buffer, Context &ctx);

    /**
     * @brief Deduce return type of expression
     * 
     */
    void deduceType(Context &ctx);
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
    Lexing::TokenType operation;

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
    UnaryExpression(const Lexing::TokenType &_operation, Expression *_expr);

    /**
     * @brief Destroy the Unary Expression object
     * 
     */
    ~UnaryExpression();

    /**
     * @brief Generate virtual machine bytecode and write it to buffer
     * 
     * @param buffer 
     */
    void generateBytecode(std::vector<VM::Byte> &buffer, Context &ctx);

    /**
     * @brief Deduce return type of expression
     * 
     */
    void deduceType(Context &ctx);
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

    /**
     * @brief Generate virtual machine bytecode and write it to buffer
     * 
     * @param buffer 
     */
    void generateBytecode(std::vector<VM::Byte> &buffer, Context &ctx);

    /**
     * @brief Deduce return type of expression
     * 
     */
    void deduceType(Context &ctx);
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

    /**
     * @brief Generate virtual machine bytecode and write it to buffer
     * 
     * @param buffer 
     */
    virtual void generateBytecode(std::vector<VM::Byte> &buffer, Context &ctx) = 0;
};

class DeclarationStatement final : public Statement {
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
     * @brief Variable name
     * 
     */
    std::string name;

    /**
     * @brief Variable type
     * 
     */
    uint64_t type;

    /**
     * @brief Initialization value
     * 
     */
    Expression *expr;

    /**
     * @brief Construct a new Declaration Statement object
     * 
     * @param _name 
     * @param _expr 
     */
    DeclarationStatement(const std::string &_name, const uint64_t _type = -1, Expression *_expr = nullptr);

    /**
     * @brief Destroy the Declaration Statement object
     * 
     */
    ~DeclarationStatement();

    /**
     * @brief Generate virtual machine bytecode and write it to buffer
     * 
     * @param buffer 
     */
    void generateBytecode(std::vector<VM::Byte> &buffer, Context &ctx);
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

    /**
     * @brief Generate virtual machine bytecode and write it to buffer
     * 
     * @param buffer 
     */
    void generateBytecode(std::vector<VM::Byte> &buffer, Context &ctx);
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

    //note this can be an if statement, representing elif chains
    /**
     * @brief Else-body
     * 
     */
    Statement *elseBody;

    /**
     * @brief Construct a new If Statement object
     * 
     * @param condition Condition to check before executing if-body or else-body
     * @param ifBody If-body
     * @param elseBody Else-body
     */
    IfStatement(Expression *condition, Statement *ifBody, Statement *elseBody);

    ~IfStatement();

    /**
     * @brief Generate virtual machine bytecode and write it to buffer
     * 
     * @param buffer 
     */
    void generateBytecode(std::vector<VM::Byte> &buffer, Context &ctx);
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

    /**
     * @brief Generate virtual machine bytecode and write it to buffer
     * 
     * @param buffer 
     */
    void generateBytecode(std::vector<VM::Byte> &buffer, Context &ctx);
};

};

#endif // GRAMMAR_H
