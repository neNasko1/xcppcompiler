#include <iostream>
#include <vector>
#include <fstream>
#include <sstream>
#include <iomanip>

#include "Lexer.h"
#include "Grammar.h"

int main() {
    Expression *finalExpr = new BinaryExpression(new LiteralExpression(Token(TokenType::NUMBER, "44")), Token(TokenType::PLUS), new BinaryExpression(new LiteralExpression(Token(TokenType::NUMBER, "4")), Token(TokenType::STAR), new LiteralExpression(Token(TokenType::NUMBER, "33"))));
    Expression *second = new UnaryExpression(Token(TokenType::MINUS), new LiteralExpression(Token(TokenType::NUMBER, "124")));
    Expression *comb = new BinaryExpression(new GroupedExpression(finalExpr), Token(TokenType::MINUS), second);

    std::cout << (*comb) << std::endl;

    std::ifstream inputCode("code.cnas");
    std::stringstream buffer;
    buffer << inputCode.rdbuf();
    Lexer lex(buffer.str());
    setupLexer(lex);
    lex.lex();
    lex.printLexed();
}
