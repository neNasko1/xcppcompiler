#include <vector>
#include <stack>

#include "Lexer.h"
#include "Grammar.h"
#include "Parser.h"

void ParserError(int line) {
    std::cerr << "Parser error at " << line << std::endl;
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
bool Parser::match(const TokenType &type) {
    if(this->tokens[this->codePtr].type == type) {
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

Expression *Parser::recogniseFunctionCall() {
    std::vector<Expression*> parameters;
    // We know that the next character is a name;
    std::string name = this->advance().rawValue;

    this->match(TokenType::L_PAREN);

    while(true) {
        auto currentToken = this->peek();

        if(currentToken.type == TokenType::R_PAREN) {
            this->advance();
            // We have found the end of function call parsing.
            break;
        } else if(!isStartOfExpression(currentToken)) {
            std::cerr << "Unexpected token in function call parameter parsing" << std::endl;
            std::cerr << currentToken << std::endl;
            ParserError(LINE());
        } else /*Start of expression*/ {
            parameters.push_back(this->recogniseExpression());
            if(this->isAtEnd()) {
                std::cerr << "Unexpected end of input" << std::endl;
                ParserError(LINE());
            }
            if(this->peek().type == TokenType::COMMA) {
                this->advance();
                // Everything is okay, we are expecting the next expression
                continue;
            } else if(this->peek().type == TokenType::R_PAREN) {
                this->advance();
                // We have found the end of function call parsing.
                break;
            } else {
                std::cerr << "Unexpected token in function call parameter separation" << std::endl << currentToken << std::endl;    
                ParserError(LINE());
            }
        }
    }   

    return new FunctionCall(name, parameters);
}

void Parser::combineTop(std::stack<Expression* > &expStack, std::stack<Token> &opStack) const {
    if(expStack.size() < 2 && opStack.size() < 1) {
        std::cerr << "Not enough operators and operands to combine expressions in AST " << std::endl;
        ParserError(LINE());
    }
    if(opStack.size() < 1) {
        std::cerr << "Not enough operators in stack" << std::endl;
        ParserError(LINE());
    }
    auto op = opStack.top(); opStack.pop();
    if(precedence[(int)op.type] == -1) {
        std::cerr << "Trying to combine top with " << std::endl << op << std::endl;
        ParserError(LINE());
    }
    if(precedence[op.type] == 7) { // These are the unary operations TODO
        if(expStack.size() < 1) {
            std::cerr << "Not enough operands for operator " << std::endl << op << std::endl;
            ParserError(LINE());
        }
        Expression *exp;
        exp = expStack.top(); expStack.pop();
        Expression *now = new UnaryExpression(op.type, exp);                 
        expStack.push(now);
    } else {
        if(expStack.size() < 2) {
            std::cerr << "Not enough operands for operator " << std::endl << op << std::endl;
            ParserError(LINE());
        }
        Expression *l, *r;
        l = expStack.top(); expStack.pop();
        r = expStack.top(); expStack.pop();
        Expression *now = new BinaryExpression(r, op.type, l);                 
        expStack.push(now);
    }
}

Expression *Parser::recogniseExpression() { 
    std::stack<Expression*> expStack; 
    std::stack<Token> opStack; 
    bool canBeUnary = true;
    int cntL_PAREN = 0; 

    while(true) {
        auto currentToken = this->peek();
        if(isSeparatorToken(currentToken)) {
            // Note codePtr shouldn't be advanced
            break;
        }
        this->advance();
        if(precedence[(int)currentToken.type] != -1) {
            if(canBeUnary && canBeUnaryOperator(currentToken)) {
                // We can and have to transform this operator token to its matching unary token
                transformToMatchingUnary(currentToken);
            }

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
            cntL_PAREN ++;

            canBeUnary = true;
            continue; // We have to continue, so we dont make canBeUnary to false;
        } else if(currentToken.type == TokenType::R_PAREN) {
            if(cntL_PAREN == 0) { // Same as separator case
                // This has to be a function call R_PAREN, so we should break
                this->codePtr --;
                break;
            }
            while(!opStack.empty() && opStack.top().type != TokenType::L_PAREN) {
                this->combineTop(expStack, opStack);
            }
            if(opStack.empty()) {
                std::cerr << "No matching left paranthesis " << std::endl;
                ParserError(LINE());
            }  

            opStack.pop();
            cntL_PAREN --;
        } else if(currentToken.type >= TokenType::CHARACTER && currentToken.type < TokenType::STRING) {
            expStack.push(new LiteralExpression(currentToken));
        } else if(currentToken.type == TokenType::NAME) {
            // If this is a function call;
            if(!this->isAtEnd() && this->peek().type == TokenType::L_PAREN) {
                this->codePtr --;
                Expression *now = this->recogniseFunctionCall();
                expStack.push(now);
            } else {
                expStack.push(new LiteralExpression(currentToken));
            }
        } else {
            std::cerr << "Unexpected token in expression parsing: " << currentToken.type << std::endl;
            ParserError(LINE());
        }
        canBeUnary = false;
    }

    while(!opStack.empty()) {
        this->combineTop(expStack, opStack);
    }
    if(expStack.size() > 1) {
        std::cerr << "Not enough operators in stack." << std::endl;
        ParserError(LINE()); 
    }
    if(expStack.size() == 0) {
        std::cerr << "Empty expression." << std::endl;
        ParserError(LINE());
    }
    return expStack.top();
}

bool isSeparatorToken(const Token &token) {
    return token.type >= TokenType::COMMA && token.type <= TokenType::QUESTION_MARK;
}

bool isStartOfExpression(const Token &token) {
    return (token.type >= TokenType::CHARACTER && token.type <= TokenType::NAME) 
    || token.type == TokenType::L_PAREN 
    || canBeUnaryOperator(token); // Token is an unary operator;
}
