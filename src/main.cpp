#include <iostream>
#include <vector>
#include <fstream>
#include <sstream>
#include <iomanip>

#include "Lexer.h"
#include "Grammar.h"
#include "Parser.h"

int main(int argc, char *argv[]) {
    if(argc == 1) {
        std::cerr << "There is no file to compile" << std::endl;
        return 0;
    }

    std::ifstream inputCode(argv[1]);
    std::stringstream buffer;
    buffer << inputCode.rdbuf();

    Lexing::Lexer lexer(buffer.str());
    Lexing::Lexer::setupBasicLexer(lexer);
    lexer.lex();

    // lex.printLexed();

    Parsing::Parser parser(lexer.lexed);
    Grammar::Statement *firstLine = (Grammar::Statement*)parser.recognizeStatementList();

    std::cout << (*firstLine) << std::endl;

    delete firstLine;
}
