#include <iostream>
#include <vector>
#include <fstream>
#include <sstream>
#include <iomanip>

#include "src/Lexer.h"
#include "src/Grammar.h"
#include "src/Parser.h"

int main() {

    Expression *finalExpr = new BinaryExpression(new LiteralExpression(Token(TokenType::NUMBER, "44")), TokenType::PLUS, new BinaryExpression(new LiteralExpression(Token(TokenType::NUMBER, "4")), TokenType::STAR, new LiteralExpression(Token(TokenType::NUMBER, "33"))));
    Expression *second = new UnaryExpression(TokenType::MINUS, new LiteralExpression(Token(TokenType::NUMBER, "124")));
    Expression *comb = new BinaryExpression(finalExpr, TokenType::MINUS, second);
    ExpressionStatement *comb2 = new ExpressionStatement(comb);

    std::cout << (*comb2) << std::endl;

    std::ifstream inputCode("code.cnas");
    std::stringstream buffer;
    buffer << inputCode.rdbuf();
    Lexer lex(buffer.str());
    setupLexer(lex);
    lex.lex();
    lex.printLexed();
}
