#include <vector>
#include "Lexer.h"
#include "Grammar.h"
#include "Parser.h"

class Statement;
class Expression;

int Parser::tabIdentation = 0;

void Parser::addTabIdentation(const int deltaTabs) {
    Parser::tabIdentation += deltaTabs;
}

std::string Parser::getTabIdentation() {
    std::string ret = "";
    for(int i = 0; i < Parser::tabIdentation; i ++) {
        ret += "  ";
    }
    return ret;
}

Parser::Parser(const std::vector<Token> &_tokens) : tokens(_tokens) {}
Parser::~Parser() {}

