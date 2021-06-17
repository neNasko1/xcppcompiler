#pragma once
#ifndef PARSER_H
#define PARSER_H

#include <vector>
#include "Lexer.h"
#include "Grammar.h"

class Statement;
class Expression;

class Parser {
private:
    std::vector<Token> tokens;
    int codePtr;
    char peek() const;
    char advance();
    bool match();
    bool isAtEnd() const;

    Statement *recogniseStatement();
    Expression *recogniseExpression();
public:
    static int tabIdentation;
    static void addTabIdentation(const int deltaTabs = 0);
    static std::string getTabIdentation();

    Parser(const std::vector<Token> &_tokens);
    ~Parser();
};

#endif // PARSER_H
