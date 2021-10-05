#include <vector>
#include <stack>

#include "Lexer.h"
#include "Grammar.h"
#include "Parser.h"

namespace Parsing {

#define HARD_MATCH(tokenType) \
    if(!this->match(tokenType)) { \
        ParserError("Unexpected token \n", this->peek(), "\n when expecting ", Lexing::TokenTypeName[tokenType], "\n"); \
    }

template <typename... T> 
static void ParserErrorPrint(T... t) {
    (std::cerr << ... << t) << "\n";
}

template <typename... T> 
void ParserError(T... t) {
    std::cerr << "There was an error while parsing " << "\n";
    ParserErrorPrint(t...);
    exit(0);
}

int32_t Parser::tabIdentation = 0;

Parser::Parser(const std::vector<Lexing::Token> &_tokens) : tokens(_tokens), codePtr(0) {}
Parser::~Parser() {}

Lexing::Token Parser::peek() const {
    return this->tokens[this->codePtr];
}
Lexing::Token Parser::advance() {
    return this->tokens[this->codePtr ++];
}
bool Parser::match(const Lexing::TokenType type) {
    if(this->tokens[this->codePtr].type == type) {
        this->codePtr ++;
        return true;
    } else {
        return false;
    }
}
//TODO: Add hardmatch function or macro

bool Parser::isAtEnd() const {
    return this->peek().type == Lexing::TokenType::END_OF_FILE;
}

void Parser::addTabIdentation(const int32_t deltaTabs) {
    Parser::tabIdentation += deltaTabs;
}

std::string Parser::getTabIdentation() {
    std::string ret = "";
    for(int32_t i = 0; i < Parser::tabIdentation; i ++) {
        ret += ",  ";
    }
    return ret;
}

Grammar::Expression *Parser::recognizeFunctionCall() {
    std::vector<Grammar::Expression*> parameters;
    // We know that the next character is a name;
    std::string name = this->advance().lexeme;

    this->match(Lexing::TokenType::L_PAREN);

    while(true) {
        auto currentToken = this->peek();

        if(currentToken.type == Lexing::TokenType::R_PAREN) {
            this->advance();
            // We have found the end of function call parsing.
            break;
        } else if(!isStartOfExpression(currentToken)) {
            ParserError("Unexpected token in function call parameter parsing", "\n", currentToken, "\n");
        } else /*Start of expression*/ {
            parameters.push_back(this->recognizeExpression());
            if(this->isAtEnd()) {
                ParserError("Unexpected end of input", "\n");
            }
            if(this->match(Lexing::TokenType::COMMA)) {
                // Everything is okay, we are expecting the next expression
                continue;
            } else if(this->match(Lexing::TokenType::R_PAREN)) {
                // We have found the end of function call parsing.
                break;
            } else {
                ParserError("Unexpected token in function call parameter separation: ", "\n", currentToken, "\n");
            }
        }
    }   

    return new Grammar::FunctionCall(name, parameters);
}

void Parser::combineTop(std::stack<Grammar::Expression* > &expStack, std::stack<Lexing::Token> &opStack) const {
    // TODO find a place for this function
    if(expStack.size() < 2 && opStack.size() < 1) {
        ParserError("Not enough operators and operands to combine expressions in AST ", "\n");
    }
    if(opStack.size() < 1) {
        ParserError("Not enough operators in stack", "\n");
    }
    auto op = opStack.top(); opStack.pop();
    if(Lexing::precedence[(int32_t)op.type] == -1) {
        ParserError("Trying to combine top with ", op, "\n");
    }
    if(Lexing::precedence[op.type] == 7) { // These are the unary operations TODO
        if(expStack.size() < 1) {
            ParserError("Not enough operands for operator ", "\n", op, "\n");
        }
        Grammar::Expression *exp;
        exp = expStack.top(); expStack.pop();
        Grammar::Expression *now = new Grammar::UnaryExpression(op.type, exp);                 
        expStack.push(now);
    } else {
        if(expStack.size() < 2) {
            ParserError("Not enough operands for operator ", "\n", op, "\n");
        }
        Grammar::Expression *l, *r;
        l = expStack.top(); expStack.pop();
        r = expStack.top(); expStack.pop();
        Grammar::Expression *now = new Grammar::BinaryExpression(r, op.type, l);                 
        expStack.push(now);
    }
}

Grammar::Expression *Parser::recognizeExpression() { 
    std::stack<Grammar::Expression*> expStack; 
    std::stack<Lexing::Token> opStack; 
    bool canBeUnary = true;
    int32_t cntL_PAREN = 0; 

    while(true) {
        if(this->isAtEnd()) {
            ParserError("Unexpected EOF, while parsing expression", "\n");
        }
        auto currentToken = this->peek();

        if(isEndOfExpression(currentToken)) {
            // Note codePtr shouldn't be advanced
            break;
        }

        this->advance();

        if(Lexing::precedence[(int32_t)currentToken.type] != -1) {
            if(canBeUnary && canBeUnaryOperator(currentToken)) {
                // We can and have to transform this operator token to its matching unary token
                transformToMatchingUnary(currentToken);
            }

            // We pop all operators with Lexing::precedence less than the current
            while(!opStack.empty() && opStack.top().type != Lexing::TokenType::L_PAREN
                && ((Lexing::precedence[opStack.top().type] < Lexing::precedence[currentToken.type]) 
                || (Lexing::precedence[opStack.top().type] == Lexing::precedence[currentToken.type] 
                    && Lexing::precedence[currentToken.type] % 2 == 0))) { // Left associative
                this->combineTop(expStack, opStack);
            } 
            opStack.push(currentToken);

            // After a unary operator we can have another one, e.g. --x
            canBeUnary = true;

            // We have to continue, so we dont make canBeUnary to false;
            continue; 
        } else if(currentToken.type == Lexing::TokenType::L_PAREN) {
            opStack.push(currentToken);
        
            // Number of nested L_PARENs should increase
            cntL_PAREN ++;

            // After a L_PAREN an unary operator can follow
            canBeUnary = true;

            // We have to continue, so we dont make canBeUnary to false;
            continue; 
        } else if(currentToken.type == Lexing::TokenType::R_PAREN) {
            if(cntL_PAREN == 0) { // Same as separator case
                // This has to be a function call R_PAREN, so we should break
                this->codePtr --;
                break;
            }

            // Pop top of operation stack while we haven't found the matching L_PAREN
            while(!opStack.empty() && opStack.top().type != Lexing::TokenType::L_PAREN) {
                this->combineTop(expStack, opStack);
            }

            if(opStack.empty()) {
                ParserError("No matching left paranthesis ", "\n");
            }  

            opStack.pop();
            
            // Number of nested L_PARENs should decrease
            cntL_PAREN --;
        } else if(currentToken.type >= Lexing::TokenType::CHARACTER && currentToken.type < Lexing::TokenType::STRING) {
            expStack.push(new Grammar::LiteralExpression(currentToken));
        } else if(currentToken.type == Lexing::TokenType::NAME) {
            if(!this->isAtEnd() && this->peek().type == Lexing::TokenType::L_PAREN) {
                // If this is a function call;
                this->codePtr --;
                Grammar::Expression *now = this->recognizeFunctionCall();
                expStack.push(now);
            } else {
                // Else if it is a variable name
                expStack.push(new Grammar::LiteralExpression(currentToken));
            }
        } else {
            ParserError("Unexpected token in expression parsing: ", Lexing::TokenTypeName[currentToken.type], "\n");
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
        ParserError("Not enough operators in stack. \n"); 
    }
    if(expStack.size() == 0) {
        // If there is no expression in the stack
        ParserError("Empty expression. \n");
    }
    return expStack.top();
}

Grammar::Statement *Parser::recognizeDeclarationStatement() {
    HARD_MATCH(Lexing::TokenType::VAR);
    std::string name = "";
    uint64_t type = -1;
    Grammar::Expression *expr = nullptr;

    if(this->peek().type != Lexing::TokenType::NAME) {
        ParserError("Unexpected token in variable declaration \n", this->peek(), "when expecting variable name ");
    } 
    name = this->advance().lexeme;

    if(this->match(Lexing::TokenType::COLON)) {
        if(this->peek().type != Lexing::TokenType::NAME) {
            ParserError("Unexpected token in variable declaration \n", this->peek(), "when expecting variable type ");
        } 

        // We have to figure out the type in the type system
        type = Grammar::Type::findType(this->advance().lexeme);        

        if(isSeparatorToken(this->peek())) {
            // All is good we are ready to return to continue
            this->advance();
        } else {
            HARD_MATCH(Lexing::TokenType::EQUAL);

            expr = this->recognizeExpression();
            if(!isSeparatorToken(this->peek())) {
                ParserError("Unexpected token in variable declaration \n", this->peek(), "when expecting variable declaration termination");
            }
            this->advance();
        }
    } else {
        // We have to deduce variable type from the expression:
        HARD_MATCH(Lexing::TokenType::EQUAL);

        // Then we have to recognize the expression
        expr = this->recognizeExpression();

        if(!isSeparatorToken(this->peek())) {
            ParserError("Unexpected token in variable declaration \n", this->peek(), "when expecting variable declaration termination");
        }
        this->advance();

        type = -1;    
    }

    return new Grammar::DeclarationStatement(name, type, expr);
}

Grammar::Statement *Parser::recognizeExpressionStatement() {
    Grammar::Expression *expr = this->recognizeExpression();

    HARD_MATCH(Lexing::TokenType::SEMICOLON);

    return new Grammar::ExpressionStatement(expr);
}

Grammar::Statement *Parser::recognizeIfStatement() {
    HARD_MATCH(Lexing::TokenType::IF);
    // Recognize condition
    Grammar::Expression *condition = this->recognizeExpression();
    // Recognize if-body
    Grammar::Statement *ifBody = this->recognizeStatementList();
    Grammar::Statement *elseBody = nullptr;

    if(this->match(Lexing::TokenType::ELSE)) {
        elseBody = this->recognizeStatement();
    }

    return new Grammar::IfStatement(condition, ifBody, elseBody);
}

Grammar::Statement *Parser::recognizeStatementList() {
    if(this->match(Lexing::TokenType::DO)) {
        return new Grammar::StatementList({this->recognizeStatement()});
    }

    HARD_MATCH(Lexing::TokenType::L_BRACE);

    std::vector<Grammar::Statement*> list;

    while(true) {
        if(this->isAtEnd()) {
            ParserError("Unexpected EOF, while parsing statement list \n");
        }
        Lexing::Token currentToken = this->peek();

        if(this->match(Lexing::TokenType::R_BRACE)) {
            // If we can match } we should exit and advance
            break;
        } else {
            list.push_back(this->recognizeStatement());
        }
    }

    return new Grammar::StatementList(list);
}

Grammar::Statement *Parser::recognizeStatement() {
    if(this->isAtEnd()) {
        ParserError("Unexpected EOF, while parsing statement \n");
    }
    Lexing::Token currentToken = this->peek();
    if(currentToken.type == Lexing::TokenType::IF) {
        // We have to recognize if
        return this->recognizeIfStatement();
    } else if(currentToken.type == Lexing::TokenType::VAR) {
        return this->recognizeDeclarationStatement();
    } else if(isStartOfStatementList(currentToken)) {
        return this->recognizeStatementList();
    } else {
        if(!isStartOfExpression(currentToken)) {
            // We are expecting an expression, but this is not the start of one
            ParserError("Unexpected token \n", currentToken, "\n while expecting start of expression \n");
            return nullptr;
        } else {
            // We need to recognize the expression
            return this->recognizeExpressionStatement();
        }
    }
    return nullptr;
}

bool isSeparatorToken(const Lexing::Token &token) {
    return token.type == Lexing::TokenType::COMMA || token.type == Lexing::TokenType::SEMICOLON;
}

bool isStartOfExpression(const Lexing::Token &token) {
    return (token.type >= Lexing::TokenType::CHARACTER && token.type <= Lexing::TokenType::NAME) 
    || token.type == Lexing::TokenType::L_PAREN 
    || canBeUnaryOperator(token); // Lexing::Token is an unary operator;
}

bool isEndOfExpression(const Lexing::Token &token) {
    return isSeparatorToken(token) || token.type == Lexing::TokenType::L_BRACE || token.type == Lexing::TokenType::DO;
}

bool isStartOfStatementList(const Lexing::Token &token) {
    return token.type == Lexing::TokenType::DO || token.type == Lexing::TokenType::L_BRACE;
}

};