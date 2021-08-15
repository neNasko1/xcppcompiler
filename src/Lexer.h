#pragma once
#ifndef LEXER_H
#define LEXER_H

#include <vector>
#include <iostream>
#include <utility>
#include <string>

const int ASCII_SIZE = 256;
enum TokenType {
    //Keywords
    ELSE, FUNCTION, FOR, IF, RETURN, VAR, WHILE,
    //Operators
    PLUS, MINUS, STAR, SLASH, MODULO, OR, AND, XOR, NOT,
    PLUS_EQUAL, MINUS_EQUAL, STAR_EQUAL, SLASH_EQUAL, MODULO_EQUAL, OR_EQUAL, AND_EQUAL, XOR_EQUAL, EQUAL,
    //Boolean operators
    BANG, BANG_EQUAL, EQUAL_EQUAL, LESS, LESS_EQUAL, GREATER, GREATER_EQUAL, OROR, ANDAND, XORXOR,
    //Unary
    UNARY_PLUS, UNARY_MINUS, UNARY_REFERENCE, UNARY_DEREFERENCE, 
    //Separators
    COMMA, SEMICOLON, DOT, COLON, QUESTION_MARK,
    //Brackets
    L_BRACE, R_BRACE, L_PAREN, R_PAREN, L_SQUARE_BRACKET, R_SQUARE_BRACKET,
    //Literals
    CHARACTER, NUMBER, BOOLEAN, STRING, NAME,
    size
};

const std::string TokenTypeName[TokenType::size] = {
    "else", "function", "for", "if", "return", "var", "while",
    "+", "-", "*", "/", "%", "|", "&", "^", "~",
    "+=", "-=", "*=", "/=", "%=", "|=", "&=", "^=", "=",
    "!", "!=", "==", "<", "<=", ">", ">=", "||", "&&", "^^",
    "unary +", "unary -", "unary &", "unary *",
    ",", ";", ".", ":", "?",
    "{", "}", "(", ")", "[", "]",
    "character", "number", "boolean", "string", "name"
};

// -1 means token is not an operator
// even precedence value means operator should be evaluated from left to right
// odd precedence value means operator should be evaluated from right to left
// https://en.cppreference.com/w/cpp/language/operator_precedence
const int precedence[TokenType::size] = {
    -1, -1, -1, -1, -1, -1, -1,
    12, 12, 10, 10, 10, 26, 22, 24, 7,
    35, 35, 35, 35, 35, 35, 35, 35, 35,
    7, 20, 20, 18, 18, 18, 18, 32, 28, 30,
    7, 7, 7, 7, // ??? SUSSY
    -1, -1, -1, -1, -1,
    -1, -1, -1, -1, -1, -1,
    -1, -1, -1, -1, -1
};

#define LINE() __LINE__
void LexerError(int line);

bool isDigit(const char c);
bool isSmallLetter(const char c);
bool isBigLetter(const char c);
bool isLetter(const char c);
bool isWhitespace(const char c);
bool isBracket(const char c);
bool isOperator(const char c);
bool isSeparator(const char c);
int  typeOfChar(const char c);

class LexerTrie {
public:
    class Node {
    public:
        Node *nxt[ASCII_SIZE];
        TokenType type;
        Node();
    };
    Node *root;
    LexerTrie();
    LexerTrie(const std::vector<std::pair<std::string, TokenType> > &_words);
    ~LexerTrie();
    void advance(Node *&curr, char c) const ;
    void addWord(const std::string &toAdd, const TokenType &type);
    int findWord(const std::string &toFind) const;
};

class Token {
public:
    TokenType type;
    std::string rawValue;
    int lineNmb, startPos;
    Token();
    Token(const TokenType &_type, const std::string &_rawValue = "", const int &_lineNmb = 0, const int &_startPos = 0);
    ~Token();
};
std::ostream& operator <<(std::ostream &out, const Token &token);

void transformToMatchingUnary(Token &token);
bool canBeUnaryOperator(const Token &token);

class Lexer {
private:
    std::string code;
    int codePtr, lineNmb, charNmb;
    LexerTrie lexTrie;
    char peek() const;
    char advance();
    bool isAtEnd() const;

    Token recognizeOperator();
    Token recognizeNumber();
    Token recognizeWord();
    Token recognizeString();
    Token recognizeChar();
public:
    std::vector<Token> lexed;
    Lexer();
    Lexer(std::string _code);
    ~Lexer();
    void addWord(const std::string &toAdd, const TokenType &type);
    void lex();
    void printLexed() const;
};

void setupLexer(Lexer &lexer);

#endif // LEXER_H
