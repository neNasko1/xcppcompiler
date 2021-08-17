#pragma once
#ifndef PARSER_H
#define PARSER_H

#include <vector>
#include <stack>
#include "Lexer.h"
#include "Grammar.h"

/**
 * @brief Print the line of the parsing error in compiler code and exit(0)
 * 
 * @param line 
 */
void ParserError(int line);

class Statement;
class Expression;

/**
 * @brief Parser class which generates AST from the input Tokens
 * 
 */
class Parser {
//private:
public:
    
    /**
     * @brief Tokens to parse
     * 
     */
    std::vector<Token> tokens;
    
    /**
     * @brief Current token being looked at
     * 
     */
    int codePtr;
    
    /**
     * @brief Look at next token in code
     * 
     * @return Token Next token
     */
    Token peek() const;
    
    /**
     * @brief Look at next character in code, and move codePtr
     * 
     * @return char Next character
     */
    Token advance();
    
    /**
     * @brief If current token matches the given type advance 
     * 
     * @param type Type to match
     * @return true if the parser pointer advanced and false otherwise
     */
    bool match(const TokenType &type);

    /**
     * @brief Check if codePtr is at the end of the input code
     * 
     * @return true if the parser pointer is at the end of input
     */
    bool isAtEnd() const;

    /**
     * @brief Execute the top operation in opStack and combine some of the top elements from expStack
     * 
     * @param expStack Stack with expressions
     * @param opStack Stack with operations
     */
    void combineTop(std::stack<Expression* > &expStack, std::stack<Token> &opStack) const;

    /**
     * @brief Recognize expression starting from the parser pointer
     * 
     * @return Expression* 
     */
    Expression *recogniseExpression();

    /**
     * @brief Recognize function call starting from the parser pointer
     * 
     * @return Expression* 
     */
    Expression *recogniseFunctionCall();

    /**
     * @brief Recognize expression statement starting from the parser pointer
     * 
     * @return Statement* Recognized statement
     */
    Statement *recogniseExpressionStatement();

    /**
     * @brief Recognize statement list starting from the parser pointer starting with a L_BRACE and ending at a R_BRACE
     * 
     * @return Statement* Recognized statement list
     */
    Statement *recogniseStatementList();

public:
    // TODO: move to a different location
    /**
     * @brief Tab identation of the current recursive printing of expression
     * 
     */
    static int tabIdentation;

    /**
     * @brief Change tab identation
     * 
     * @param deltaTabs Amount of tab identation to add
     */
    static void addTabIdentation(const int deltaTabs = 0);

    /**
     * @brief Get the Tab Identation value
     * 
     * @return std::string Tab Identation string
     */
    static std::string getTabIdentation();

    /**
     * @brief Construct a new Parser object
     * 
     * @param _tokens 
     */
    Parser(const std::vector<Token> &_tokens);

    /**
     * @brief Destroy the Parser object
     * 
     */
    ~Parser();
};

/**
 * @brief 
 * 
 * @param token 
 * @return true if token can be start of an expression and false otherwise
 */
bool isStartOfExpression(const Token &token);

/**
 * @brief 
 * 
 * @param token 
 * @return true if token can be start of an expression and false otherwise
 */
bool isSeparatorToken(const Token &token);

#endif // PARSER_H
