#pragma once
#ifndef LEXER_H
#define LEXER_H

#include <vector>
#include <iostream>
#include <utility>
#include <string>

namespace Lexing {

#define LINE() __LINE__

/**
 * @brief TokenType including all types of tokens in the language
 * 
 */
const int32_t ASCII_SIZE = 256;
enum TokenType {
    //Keywords
    ELSE, FUNCTION, FOR, IF, RETURN, VAR, WHILE, DO,
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
    END_OF_FILE,
    size
};

/**
 * @brief Utility array which helps with outputting real names of the TokenType enum
 * 
 */
const std::string TokenTypeName[TokenType::size] = {
    "else", "function", "for", "if", "return", "var", "while", "do",
    "+", "-", "*", "/", "%", "|", "&", "^", "~",
    "+=", "-=", "*=", "/=", "%=", "|=", "&=", "^=", "=",
    "!", "!=", "==", "<", "<=", ">", ">=", "||", "&&", "^^",
    "unary +", "unary -", "unary &", "unary *",
    ",", ";", ".", ":", "?",
    "{", "}", "(", ")", "[", "]",
    "character", "number", "boolean", "string", "name",
    "EOF"
};

/**
 * @brief Array containing all precedence levels and associativities of all tokens
 * (-1 means token is not an operator)
 * (parity of precedence gives information about associativity - even-left odd-right)
 * 
 */
const int32_t precedence[TokenType::size] = {
    -1, -1, -1, -1, -1, -1, -1, -1,
    12, 12, 10, 10, 10, 26, 22, 24, 7,
    35, 35, 35, 35, 35, 35, 35, 35, 35,
    7, 20, 20, 18, 18, 18, 18, 32, 28, 30,
    7, 7, 7, 7, // ??? Not sure
    -1, -1, -1, -1, -1,
    -1, -1, -1, -1, -1, -1,
    -1, -1, -1, -1, -1,
    -1,
};

/**
 * @brief Print the parameters given and exit
 * 
 * @param T 
 */
template <typename... T> 
void LexerError(T... t);

bool isDigit(const char c);
bool isSmallLetter(const char c);
bool isBigLetter(const char c);
bool isLetter(const char c);
bool isWhitespace(const char c);
bool isBracket(const char c);
bool isOperator(const char c);
bool isSeparator(const char c);
int32_t  typeOfChar(const char c);

/**
 * @brief Trie structure supporting adding words and finding words in O(len)
 * 
 */
class LexerTrie {
public:

    /**
     * @brief Utility class containing information about single nodes in trie
     * 
     */
    class Node {
    public:

        /**
         * @brief Children in trie
         * 
         */
        Node *nxt[ASCII_SIZE];

        /**
         * @brief Type of word ending in this Node
         * 
         */
        TokenType type;

        /**
         * @brief Construct a new Node object
         * 
         */
        Node();

        /**
         * @brief Destroy the Node object
         * 
         */
        ~Node();
    };

    /**
     * @brief Root node of the trie
     * 
     */
    Node *root;
    
    /**
     * @brief Construct a new Lexer Trie object
     * 
     */
    LexerTrie();
    
    /**
     * @brief Construct a new Lexer Trie object
     * 
     * @param _words Words to add in the trie
     */
    LexerTrie(const std::vector<std::pair<std::string, TokenType> > &_words);
    
    /**
     * @brief Destroy the Lexer Trie object
     * 
     */
    ~LexerTrie();
    
    /**
     * @brief Advance from a node to its child following the link with letter c
     * 
     * @param curr Node to go down from
     * @param c Letter to follow
     */
    void advance(Node *&curr, char c) const ;
    
    /**
     * @brief Add word to trie
     * 
     * @param toAdd Word to add to trie
     * @param type TokenType of word
     */
    void addWord(const std::string &toAdd, const TokenType &type);
    
    /**
     * @brief Find word in trie
     * 
     * @param toFind Word to find in trie
     * @return TokenType type of word if found 
     */
    TokenType findWord(const std::string &toFind) const;
};

/**
 * @brief Class token containing the information about a token
 * 
 */
class Token {
public:

    /**
     * @brief Type of the token 
     * 
     */
    TokenType type;

    /**
     * @brief Lexeme of the token
     * 
     */
    std::string lexeme;

    /**
     * @brief Line number in file
     * 
     */
    int32_t lineNmb;

    /**
     * @brief Position in line
     * 
     */
    int32_t startPos;

    /**
     * @brief Construct a new Token object
     * 
     */
    Token();

    /**
     * @brief Construct a new Token object
     * 
     * @param _type Type of the token 
     * @param _lexeme Lexeme of the token
     * @param _lineNmb Line number in file
     * @param _startPos Position in line
     */
    Token(const TokenType &_type, const std::string &_lexeme = "", const int32_t &_lineNmb = 0, const int32_t &_startPos = 0);

    /**
     * @brief Destroy the Token object
     * 
     */
    ~Token();
};

/**
 * @brief Operation overloading which makes it possible to output a token to std::ostream
 * 
 * @param os Ostream to output to 
 * @param token Token to output
 * @return std::ostream& Modified stream
 */
std::ostream& operator <<(std::ostream &os, const Token &token);

/**
 * @brief Given a operator token, if it has a matching unary operator transform it to it, e.g. + -> unary+
 * 
 * @param token Token to transform
 */
void transformToMatchingUnary(Token &token);

/**
 * @brief Returns whether the token can be an unary operator
 * 
 * @param token Token to check
 * @return true if token is an operator and the operator can be unary and false otherwise
 */
bool canBeUnaryOperator(const Token &token);

/**
 * @brief Lexer class which generates vector of tokens from the input code
 * 
 */
class Lexer {
private:
    /**
     * @brief Input code
     * 
     */
    std::string code;

    /**
     * @brief Current character being looked at
     * 
     */
    int32_t codePtr;

    /**
     * @brief Current line number
     * 
     */
    int32_t lineNmb;

    /**
     * @brief Current char number in current line
     * 
     */
    int32_t charNmb;

    /**
     * @brief LexTrie which contains all reserved words and operators
     * 
     */
    LexerTrie lexTrie;

    /**
     * @brief Look at next character in code
     * 
     * @return char Next character
     */
    char peek() const;

    /**
     * @brief Look at next character in code, and move codePtr
     * 
     * @return char Next character
     */
    char advance();

    /**
     * @brief Check if codePtr is at the end of the input code
     * 
     * @return true if lexer pointer is at the end of input and false otherwise 
     */
    bool isAtEnd() const;

    /**
     * @brief Recognize operator starting from codePtr
     * 
     * @return Token Recognized operator
     */
    Token recognizeOperator();

    /**
     * @brief Recognize number starting from codePtr
     * 
     * @return Token Recognized number
     */
    Token recognizeNumber();

    /**
     * @brief Recognize word starting from codePtr
     * 
     * @return Token Recognized word
     */
    Token recognizeWord();

    /**
     * @brief Recognize string starting from codePtr
     * 
     * @return Token Recognized string
     */
    Token recognizeString();

    /**
     * @brief Recognize char starting from codePtr
     * 
     * @return Token Recognized char
     */
    Token recognizeChar();

public:
    /**
     * @brief Lexed tokens(output)
     * 
     */
    std::vector<Token> lexed;

    /**
     * @brief Construct a new Lexer object
     * 
     */
    Lexer();

    /**
     * @brief Construct a new Lexer object
     * 
     * @param _code Code to give to lexer to lex  
     */
    Lexer(std::string _code);

    /**
     * @brief Destroy the Lexer object
     * 
     */
    ~Lexer();
    
    /**
     * @brief Add a reserved-word to lexTrie(do this for all Keywords, operators...)
     * 
     * @param toAdd Word to add to trie
     * @param type Type of word to add to trie
     */
    void addWord(const std::string &toAdd, const TokenType &type);

    /**
     * @brief Run lexing process
     * 
     */
    void lex();

    /**
     * @brief Utility function to print lexed tokens
     * 
     */
    void printLexed() const;
};

/**
 * @brief Utility function to setup lexer for cnas
 * 
 * @param lexer Lexer to setup
 */
void setupLexer(Lexer &lexer);

};

#endif // LEXER_H
