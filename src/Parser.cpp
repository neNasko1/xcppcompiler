#include <vector>
#include <stack>

#include "Lexer.h"
#include "Grammar.h"
#include "Parser.h"

#define HARD_MATCH(tokenType) \
    if(!this->match(tokenType)) { \
        std::cerr << "Unexpected token " << std::endl << this->peek() << std::endl; \
        std::cerr << "when expecting " << TokenTypeName[tokenType] << std::endl; \
        ParserError(LINE()); \
    }

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
bool Parser::match(const TokenType type) {
    if(this->tokens[this->codePtr].type == type) {
        this->codePtr ++;
        return true;
    } else {
        return false;
    }
}
//TODO: Add hardmatch function or macro

bool Parser::isAtEnd() const {
    return this->peek().type == TokenType::END_OF_FILE;
}

void Parser::addTabIdentation(const int deltaTabs) {
    Parser::tabIdentation += deltaTabs;
}

std::string Parser::getTabIdentation() {
    std::string ret = "";
    for(int i = 0; i < Parser::tabIdentation; i ++) {
        ret += ",  ";
    }
    return ret;
}

Expression *Parser::recognizeFunctionCall() {
    std::vector<Expression*> parameters;
    // We know that the next character is a name;
    std::string name = this->advance().lexeme;

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
            parameters.push_back(this->recognizeExpression());
            if(this->isAtEnd()) {
                std::cerr << "Unexpected end of input" << std::endl;
                ParserError(LINE());
            }
            if(this->match(TokenType::COMMA)) {
                // Everything is okay, we are expecting the next expression
                continue;
            } else if(this->match(TokenType::R_PAREN)) {
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

Expression *Parser::recognizeExpression() { 
    std::stack<Expression*> expStack; 
    std::stack<Token> opStack; 
    bool canBeUnary = true;
    int cntL_PAREN = 0; 

    while(true) {
        if(this->isAtEnd()) {
            std::cerr << "Unexpected EOF, while parsing expression" << std::endl;
            ParserError(LINE());
        }
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

            // We pop all operators with precedence less than the current
            while(!opStack.empty() && opStack.top().type != TokenType::L_PAREN
                && ((precedence[opStack.top().type] < precedence[currentToken.type]) 
                || (precedence[opStack.top().type] == precedence[currentToken.type] 
                    && precedence[currentToken.type] % 2 == 0))) { // Left associative
                this->combineTop(expStack, opStack);
            } 
            opStack.push(currentToken);

            // After a unary operator we can have another one, e.g. --x
            canBeUnary = true;

            // We have to continue, so we dont make canBeUnary to false;
            continue; 
        } else if(currentToken.type == TokenType::L_PAREN) {
            opStack.push(currentToken);
        
            // Number of nested L_PARENs should increase
            cntL_PAREN ++;

            // After a L_PAREN an unary operator can follow
            canBeUnary = true;

            // We have to continue, so we dont make canBeUnary to false;
            continue; 
        } else if(currentToken.type == TokenType::R_PAREN) {
            if(cntL_PAREN == 0) { // Same as separator case
                // This has to be a function call R_PAREN, so we should break
                this->codePtr --;
                break;
            }

            // Pop top of operation stack while we haven't found the matching L_PAREN
            while(!opStack.empty() && opStack.top().type != TokenType::L_PAREN) {
                this->combineTop(expStack, opStack);
            }

            if(opStack.empty()) {
                std::cerr << "No matching left paranthesis " << std::endl;
                ParserError(LINE());
            }  

            opStack.pop();
            
            // Number of nested L_PARENs should decrease
            cntL_PAREN --;
        } else if(currentToken.type >= TokenType::CHARACTER && currentToken.type < TokenType::STRING) {
            expStack.push(new LiteralExpression(currentToken));
        } else if(currentToken.type == TokenType::NAME) {
            if(!this->isAtEnd() && this->peek().type == TokenType::L_PAREN) {
                // If this is a function call;
                this->codePtr --;
                Expression *now = this->recognizeFunctionCall();
                expStack.push(now);
            } else {
                // Else if it is a variable name
                expStack.push(new LiteralExpression(currentToken));
            }
        } else {
            std::cerr << "Unexpected token in expression parsing: " << currentToken.type << std::endl;
            ParserError(LINE());
        }

        // Next operator cannot be unary
        canBeUnary = false;
    }

    // Pop all operations from the stack
    while(!opStack.empty()) {
        this->combineTop(expStack, opStack);
    }
    
    if(expStack.size() > 1) {
        // If there are too many expressions in the stack
        std::cerr << "Not enough operators in stack." << std::endl;
        ParserError(LINE()); 
    }
    if(expStack.size() == 0) {
        // If there is no expression in the stack
        std::cerr << "Empty expression." << std::endl;
        ParserError(LINE());
    }
    return expStack.top();
}

Statement *Parser::recognizeExpressionStatement() {
    Expression *expr = this->recognizeExpression();

    HARD_MATCH(TokenType::SEMICOLON);

    return new ExpressionStatement(expr);
}

Statement *Parser::recognizeIfStatement() {
    HARD_MATCH(TokenType::IF);
    HARD_MATCH(TokenType::L_PAREN);    
    // Recognize condition
    Expression *condition = this->recognizeExpression();
    HARD_MATCH(TokenType::R_PAREN); 
    // Recognize if-body
    Statement *ifBody = this->recognizeStatementList();
    Statement *elseBody = nullptr;

    if(this->match(TokenType::ELSE)) {
        if(this->peek().type == TokenType::IF) {
            elseBody = this->recognizeIfStatement();
        } else {
            elseBody = this->recognizeStatementList();
        }
    }

    return new IfStatement(condition, ifBody, elseBody);
}

StatementList *Parser::recognizeStatementList() {
    HARD_MATCH(TokenType::L_BRACE);

    std::vector<Statement*> list;

    while(true) {
        if(this->isAtEnd()) {
            std::cerr << "Unexpected EOF, while parsing statement list" << std::endl;
            ParserError(LINE());
        }
        Token currentToken = this->peek();

        if(this->match(TokenType::R_BRACE)) {
            // If we can match } we should exit and advance
            break;
        } else if(currentToken.type == TokenType::IF) {
            // We have to recognize if
            list.push_back(this->recognizeIfStatement());
        } else if(!isStartOfExpression(currentToken)) {
            // We are expecting an expression, but this is not the start of one
            std::cerr << "Unexpected token " << std::endl << currentToken << std::endl;
            std::cerr << "while expecting start of expression" << std::endl;
            ParserError(LINE());
        } else {
            // We need to recognize the expression
            list.push_back(this->recognizeExpressionStatement());
        }
    }

    return new StatementList(list);
}

bool isSeparatorToken(const Token &token) {
    return token.type >= TokenType::COMMA && token.type <= TokenType::QUESTION_MARK;
}

bool isStartOfExpression(const Token &token) {
    return (token.type >= TokenType::CHARACTER && token.type <= TokenType::NAME) 
    || token.type == TokenType::L_PAREN 
    || canBeUnaryOperator(token); // Token is an unary operator;
}
