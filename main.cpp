#include <iostream>
#include <vector>
#include <fstream>
#include <sstream>
#include <iomanip>

#include "src/Lexer.h"
#include "src/Grammar.h"
#include "src/Parser.h"
#include "src/VirtualMachine.h"

int main(int argc, char *argv[]) {
    std::vector<VM::Byte> code = {
        VM::InstructionType::INT64_LOAD, 120,
        VM::InstructionType::INT64_LOAD, 40,
        VM::InstructionType::INT64_LOAD, 20,
        VM::InstructionType::INT64_DIVIDE,
        VM::InstructionType::INT64_DIVIDE,
        VM::InstructionType::INT64_LOAD, 59,
        VM::InstructionType::INT64_ADD,
        VM::InstructionType::PRINT
    };

    VM::VirtualMachine vm(code);
    vm.execute();

    return 0;

    if(argc == 1) {
        std::cerr << "There is no file to compile" << std::endl;
        return 0;
    }

    std::ifstream inputCode(argv[1]);
    std::stringstream buffer;
    buffer << inputCode.rdbuf();
    
    Lexing::Lexer lexer(buffer.str());
    Lexing::setupLexer(lexer);
    lexer.lex();
    
    // lex.printLexed();

    Parsing::Parser parser(lexer.lexed);
    Grammar::Statement *firstLine = (Grammar::Statement*)parser.recognizeStatementList();

    std::cout << (*firstLine) << std::endl;

    delete firstLine;
}
