#include <iostream>
#include <vector>
#include <fstream>
#include <sstream>
#include <iomanip>

#include "src/Lexer.h"
#include "src/Grammar.h"
#include "src/Parser.h"

int main(int argc, char *argv[]) {
    if(argc == 1) {
        std::cerr << "There is no file to compile" << std::endl;
        return 0;
    }

    std::ifstream inputCode(argv[1]);
    std::stringstream buffer;
    buffer << inputCode.rdbuf();
    Lexer lexer(buffer.str());
    setupLexer(lexer);
    lexer.lex();
    
    // lex.printLexed();

    Parser parser(lexer.lexed);
    Statement *firstLine = parser.recogniseStatementList();

    std::cout << (*firstLine) << std::endl;


}
