#pragma once
#ifndef PARSER_H
#define PARSER_H

#include <vector>
#include <stack>
#include "Lexer.h"
#include "Grammar.h"

void ParserError(int line);

class Statement;
class Expression;

class Parser {
//private:
public:
    std::vector<Token> tokens;
    int codePtr;
    Token peek() const;
    Token advance();
    bool match(const Token &token);
    bool isAtEnd() const;

    Statement *recogniseStatement();
    
    void combineTop(std::stack<Expression* > &expStack, std::stack<Token> &opStack) const;
    Expression *recogniseExpression();
public:
    static int tabIdentation;
    static void addTabIdentation(const int deltaTabs = 0);
    static std::string getTabIdentation();

    Parser(const std::vector<Token> &_tokens);
    ~Parser();
};

#endif // PARSER_H
