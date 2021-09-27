#pragma once
#ifndef PARSER_H
#define PARSER_H

#include <vector>
#include <stack>
#include "Lexer.h"

#include "Grammar.h"

namespace Parsing {

/**
 * @brief Print the parameters given and exit
 * 
 * @param T 
 */
template <typename... T> 
void ParserError(T... t);

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
    std::vector<Lexing::Token> tokens;
    
    /**
     * @brief Current token being looked at
     * 
     */
    int32_t codePtr;
    
    /**
     * @brief Look at next token in code
     * 
     * @return Lexing::Token Next token
     */
    Lexing::Token peek() const;
    
    /**
     * @brief Look at next character in code, and move codePtr
     * 
     * @return char Next character
     */
    Lexing::Token advance();
    
    /**
     * @brief Advance current token matches the given type 
     * 
     * @param type Type to match agains
     * @return true if the parser pointer advanced and false otherwise
     */
    bool match(const Lexing::TokenType type);

    /**
     * @brief Advance if current token matches the given type exit with parser error otherwise
     * 
     * @param type Type to match agains
     * @return true If there was no parsing error
     */
    bool hardMatch(const Lexing::TokenType type);

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
    void combineTop(std::stack<Grammar::Expression* > &expStack, std::stack<Lexing::Token> &opStack) const;

    /**
     * @brief Recognize expression starting from the parser pointer
     * 
     * @return Grammar::Expression* Recognized expression
     */
    Grammar::Expression *recognizeExpression();

    /**
     * @brief Recognize function call starting from the parser pointer
     * 
     * @return Grammar::Expression* Recognized function call
     */
    Grammar::Expression *recognizeFunctionCall();

    /**
     * @brief Recognize expression statement starting from the parser pointer
     * 
     * @return Grammar::Statement* Recognized statement
     */
    Grammar::Statement *recognizeExpressionStatement();

    /**
     * @brief Recognize declaration statement starting from the parser pointer
     * 
     * @return Grammar::Statement* Recognized if statement
     */
    Grammar::Statement *recognizeDeclarationStatement();

    /**
     * @brief Recognize if statement starting from the parser pointer
     * 
     * @return Grammar::Statement* Recognized if statement
     */
    Grammar::Statement *recognizeIfStatement();

    /**
     * @brief Recognize statement list starting from the parser pointer starting with a L_BRACE and ending at a R_BRACE
     * 
     * @return Grammar::Statement* Recognized statement list
     */
    Grammar::Statement *recognizeStatementList();

    /**
     * @brief Recognize statement - this is the general statement recognition
     *  
     * @return Grammar::Statement* Recognized final statement
     */
    Grammar::Statement *recognizeStatement();

public:
    // TODO: move to a different location
    /**
     * @brief Tab identation of the current recursive printing of expression
     * 
     */
    static int32_t tabIdentation;

    /**
     * @brief Change tab identation
     * 
     * @param deltaTabs Amount of tab identation to add
     */
    static void addTabIdentation(const int32_t deltaTabs = 0);

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
    Parser(const std::vector<Lexing::Token> &_tokens);

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
bool isStartOfExpression(const Lexing::Token &token);

/**
 * @brief 
 * 
 * @param token 
 * @return true if token can be start of an expression and false otherwise
 */
bool isSeparatorToken(const Lexing::Token &token);

/**
 * @brief 
 * 
 * @param token 
 * @return true if token can be end of an expression and false otherwise
 */
bool isEndOfExpression(const Lexing::Token &token);

/**
 * @brief 
 * 
 * @param token 
 * @return true if token can be start of an statementlist and false otherwise
 */
bool isStartOfStatementList(const Lexing::Token &token);

};

#endif // PARSER_H