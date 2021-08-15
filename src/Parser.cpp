#include <vector>
#include <stack>

#include "Lexer.h"
#include "Grammar.h"
#include "Parser.h"

class Statement;
class Expression;

void ParserError(int line) {
    std::cout << "Parser error at " << line << std::endl;
    exit(0);
}

int Parser::tabIdentation = 0;

Parser::Parser(const std::vector<Token> &_tokens) : tokens(_tokens), codePtr(0) {}
Parser::~Parser() {}

Token Parser::peek() const {
    return this->tokens[this->codePtr];
}
Token Parser::advance() {
    return this->tokens[this->codePtr ++];
}
bool Parser::match(const Token &token) {
    if(this->tokens[this->codePtr].type == token.type) {
        this->codePtr ++;
        return true;
    } else {
        return false;
    }
}
bool Parser::isAtEnd() const {
    return this->codePtr >= this->tokens.size();
}

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

void Parser::combineTop(std::stack<Expression* > &expStack, std::stack<Token> &opStack) const {
    if(expStack.size() < 2 && opStack.size() < 1) {
        std::cout << "Not enough operators and operands to combine expressions in AST " << std::endl;
        ParserError(LINE());
    }
    if(opStack.size() < 1) {
        std::cout << "Not enough operators in stack" << std::endl;
        ParserError(LINE());
    }
    auto op = opStack.top(); opStack.pop();
    if(precedence[(int)op.type] == -1) {
        std::cout << "Trying to combine top with " << std::endl << op << std::endl;
        ParserError(LINE());
    }
    if(precedence[op.type] == 7) { // These are the unary operations TODO
        if(expStack.size() < 1) {
            std::cout << "Not enough operands for operator " << std::endl << op << std::endl;
            ParserError(LINE());
        }
        Expression *exp;
        exp = expStack.top(); expStack.pop();
        Expression *now = new UnaryExpression(op.type, exp);                 
        expStack.push(now);
    } else {
        if(expStack.size() < 2) {
            std::cout << "Not enough operands for operator " << std::endl << op << std::endl;
            ParserError(LINE());
        }
        Expression *l, *r;
        l = expStack.top(); expStack.pop();
        r = expStack.top(); expStack.pop();
        Expression *now = new BinaryExpression(r, op.type, l);                 
        std::cout << (*now) << std::endl;
        expStack.push(now);
    }
}

Expression *Parser::recogniseExpression() { 
    std::stack<Expression* > expStack; 
    std::stack<Token> opStack; 
    bool canBeUnary = true;

    while(true) {
        auto currentToken = this->peek();
        std::cout << "Now currentToken " << currentToken << std::endl;
        // Is separator
        // TODO: Move to a different function
        if(currentToken.type >= TokenType::COMMA && currentToken.type <= TokenType::QUESTION_MARK) {
            break;
        }
        this->advance();
        if(precedence[(int)currentToken.type] != -1) {
            if(canBeUnary && canBeUnaryOperator(currentToken)) {
                transformToMatchingUnary(currentToken);
            }

            std::cout << "Here" << std::endl;
            while(!opStack.empty() && opStack.top().type != TokenType::L_PAREN
                && ((precedence[opStack.top().type] < precedence[currentToken.type]) 
                || (precedence[opStack.top().type] == precedence[currentToken.type] 
                    && precedence[currentToken.type] % 2 == 0))) { // Left associative
                this->combineTop(expStack, opStack);
            } 
            opStack.push(currentToken);
            canBeUnary = true;
            continue;
        } else if(currentToken.type == TokenType::L_PAREN) {
            opStack.push(currentToken);
            canBeUnary = true;
            continue; // We have to continue, so we dont make canBeUnary to false;
        } else if(currentToken.type == TokenType::R_PAREN) {
            while(!opStack.empty() && opStack.top().type != TokenType::L_PAREN) {
                this->combineTop(expStack, opStack);
            }
            if(opStack.empty()) {
                std::cout << "No matching left paranthesis " << std::endl;
                ParserError(LINE());
            }  
            opStack.pop();
        } else if(currentToken.type >= TokenType::CHARACTER && currentToken.type <= TokenType::NAME) {
            expStack.push(new LiteralExpression(currentToken));
        } else {
            std::cout << "Unexpected token in expression parsing: " << currentToken.type << std::endl;
            ParserError(LINE());
        }
        canBeUnary = false;
    }

    while(!opStack.empty()) {
        this->combineTop(expStack, opStack);
    }
    if(expStack.size() > 1) {
        std::cout << "Not enough operators in stack." << std::endl;
        ParserError(LINE()); 
    }
    if(expStack.size() == 0) {
        std::cout << "Empty expression." << std::endl;
        ParserError(LINE());
    }
    return expStack.top();
}

