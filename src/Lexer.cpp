#include <vector>
#include <utility>
#include <string>
#include <iomanip>

#include "Lexer.h"

namespace Lexing {

bool isDigit(const char c) { return '0' <= c && c <= '9'; }
bool isSmallLetter(const char c) { return 'a' <= c && c <= 'z'; }
bool isBigLetter(const char c) { return 'A' <= c && c <= 'Z'; }
bool isLetter(const char c) { return isSmallLetter(c) || isBigLetter(c); }
bool isWhitespace(const char c) { return c == ' ' || c == '\n' || c == '\t'; }
bool isBracket(const char c) { return c == '(' || c == ')' || c == '{' || c == '}' || c == '[' || c == ']'; }
bool isOperator(const char c) { return c == '+' || c == '-' || c == '*' || c == '/' || c == '%' || c == '&' || c == '|' || c == '^' || c == '~' || c == '=' || c == '<' || c == '>' || c == '!' || c == ','; }
bool isSeparator(const char c) { return c == ';' || c == ':' || c == '.' || c == '?'; }

int typeOfChar(const char c) {
    if(isWhitespace(c)) {
        return -1; //Whitespace
    } else if(isDigit(c) || isLetter(c)) {
        return 1; //Digit or letter
    } else if(isOperator(c)) {
        return 2; //Operator
    } else if(isBracket(c)) {
        return 3; // Bracket
    } else if(isSeparator(c)) {
        return 4; // Separator
    }
    LexerError("Unknown type of char ", c, "\n");
    return -1;
}

template <typename... T> 
void LexerErrorPrint(T... t) {
    (std::cerr << ... << t) << "\n";
}

template <typename... T> 
void LexerError(T... t) {
    std::cerr << "There was an error while parsing " << "\n";
    LexerErrorPrint(t...);
    exit(0);
}

/***********************LexerTrie class**********************/
LexerTrie::Node::Node() : type(TokenType::NAME) {
    for(size_t i = 0; i < ASCII_SIZE; i ++) {
        this->nxt[i] = nullptr;
    }
}

LexerTrie::Node::~Node() {
    for(size_t i = 0; i < ASCII_SIZE; i ++) {
        delete this->nxt[i];
    }
}

void LexerTrie::advance(Node *&curr, const char c) const {
    if(!curr || !curr->nxt[(int)c]) {
        curr = nullptr;
        return;
    }
    curr = curr->nxt[(int)c];
}

LexerTrie::LexerTrie() {root = new Node();}
LexerTrie::LexerTrie(const std::vector<std::pair<std::string, TokenType> > &_words) {
    for(auto &it : _words) {
        this->addWord(it.first, it.second);
    }
    this->root = new Node();
}
LexerTrie::~LexerTrie() {delete root;}

void LexerTrie::addWord(const std::string &toAdd, const TokenType &type) {
    auto currNode = this->root;
    for(auto &it : toAdd) {
        if(currNode->nxt[(int)it]) {
            currNode = currNode->nxt[(int)it];
        } else {
            currNode->nxt[(int)it] = new Node();
            currNode = currNode->nxt[(int)it];
        }
    }
    currNode->type = type;
}

TokenType LexerTrie::findWord(const std::string &toFind) const {
    auto currNode = this->root;
    for(auto &it : toFind) {
        currNode = currNode->nxt[(int)it];
        if(!currNode) {return NAME;}
    }
    return currNode->type;
}

/***********************Token class*************************/
Token::Token() {}
Token::Token(const TokenType &_type, const std::string &_lexeme, const int &_lineNmb, const int &_startPos) 
            : type(_type), lexeme(_lexeme), lineNmb(_lineNmb), startPos(_startPos) {}
Token::~Token() {}

std::ostream& operator <<(std::ostream &os, const Token &token) {
    return os << token.lineNmb << ", " << std::setw(7) << token.startPos << "| " << std::setw(15) << token.lexeme << "| " << std::setw(15) << TokenTypeName[token.type] << "\n";
}

bool canBeUnaryOperator(const Token &token) {
    return token.type == TokenType::PLUS || token.type == TokenType::BANG || token.type == TokenType::AND 
        || token.type == TokenType::NOT || token.type == TokenType::STAR || token.type == TokenType::MINUS;
}

void transformToMatchingUnary(Token &token) {
    if(token.type == TokenType::PLUS) {
        token.type = TokenType::UNARY_PLUS;
    } else if(token.type == TokenType::MINUS) {
        token.type = TokenType::UNARY_MINUS;
    } else if(token.type == TokenType::STAR) {
        token.type = TokenType::UNARY_DEREFERENCE;
    } else if(token.type == TokenType::AND) {
        token.type = TokenType::UNARY_REFERENCE;
    } 
}

/***********************Lexer class ************************/
Lexer::Lexer() : code(), codePtr(0), lineNmb(0), charNmb(0), lexTrie() {}
Lexer::Lexer(std::string _code) : code(_code), codePtr(0), lineNmb(0), charNmb(0), lexTrie() {}
Lexer::~Lexer() {}

char Lexer::peek() const { return code[codePtr]; }
char Lexer::advance() {
    if(code[codePtr] == '\n') {
        this->lineNmb ++;
        this->charNmb = 0;
    } else {
        this->charNmb ++;
    }
    return this->code[codePtr ++];
}
bool Lexer::isAtEnd() const { return codePtr == (int)code.size(); }

void Lexer::addWord(const std::string &toAdd, const TokenType &type) {
    lexTrie.addWord(toAdd, type);
}

Token Lexer::recognizeOperator() {
    LexerTrie::Node *currentNode = lexTrie.root;
    while(!this->isAtEnd()) {
        char current = this->peek();
        if(!currentNode->nxt[(int)current]) {
            break;
        }
        this->lexTrie.advance(currentNode, current);
        this->advance();
    }
    return Token(currentNode->type);
}

Token Lexer::recognizeNumber() {
    std::string numberValue = "";
    while(!this->isAtEnd()) {
        char current = this->peek();
        if(!isDigit(current)) {
            break;
        }
        numberValue.push_back(current);
        this->advance();
    }
    return Token(TokenType::NUMBER, numberValue);
}

Token Lexer::recognizeWord() {
    std::string nameValue = "";
    LexerTrie::Node *currentNode = lexTrie.root;
    while(!this->isAtEnd()) {
        char current = this->peek();
        if(!isLetter(current) && !isDigit(current)) {
            break;
        }
        nameValue.push_back(current);
        this->lexTrie.advance(currentNode, current);
        this->advance();
    }
    if(!currentNode || currentNode->type == TokenType::NAME) {
        return Token(TokenType::NAME, nameValue);
    } else {
        return Token(currentNode->type);
    }
}

Token Lexer::recognizeString() {
    std::string stringValue = "";
    this->advance();
    while(!this->isAtEnd()) {
        char current = this->peek();
        if(current == '"') {
            this->advance();
            break;
        }
        stringValue.push_back(current);
        this->advance();
    }
    if(this->isAtEnd()) {
        LexerError("String literal not closed \n");
    }
    return Token(TokenType::STRING, stringValue);
}

Token Lexer::recognizeChar() {
    std::string charValue = "";
    this->advance();
    if(this->isAtEnd()) {
        LexerError("Char not closed \n");
    }
    charValue.push_back(this->advance());
    if(this->isAtEnd() || this->peek() != '\'') {
        LexerError("Char not closed \n");
    }
    this->advance();
    return Token(TokenType::CHARACTER, charValue);
}

void Lexer::lex() {
    this->lexed.resize(0);
    while(!this->isAtEnd()) {
        char currentChar = this->peek(); int startPos = this->charNmb;

        Token currentToken;
        if(isWhitespace(currentChar)) {
            this->advance();
            continue;
        } if(isOperator(currentChar) || isSeparator(currentChar) || isBracket(currentChar)) {
            currentToken = this->recognizeOperator();
        } else if(isDigit(currentChar)) {
            currentToken = this->recognizeNumber();
        } else if(isLetter(currentChar)) {
            currentToken = this->recognizeWord();
        } else if(currentChar == '"') {
            currentToken = this->recognizeString();
        } else if(currentChar == '\'') {
            currentToken = this->recognizeChar();
        } else {
            LexerError("Lexing error: Found character ", currentChar, " at ", this->lineNmb, " ", this->charNmb, "\n");
        }
        currentToken.lineNmb = this->lineNmb;
        currentToken.startPos = startPos;
        this->lexed.push_back(currentToken);
    }
    this->lexed.push_back(Token(TokenType::END_OF_FILE, "", -1, -1));
}

void Lexer::printLexed() const {
    std::ios init(NULL);
    init.copyfmt(std::cout);
    for(auto &it : this->lexed) {
        std::cout << it;
    }
    std::cout.copyfmt(init);
}

void setupLexer(Lexer &lexer) {
    std::vector<std::pair<std::string, TokenType> > stringToTokentype = {
        //Keywords
        {"else", TokenType::ELSE}, {"function", TokenType::FUNCTION}, {"function", TokenType::FUNCTION},
        {"for", TokenType::FOR}, {"if", TokenType::IF}, {"return", TokenType::RETURN}, {"while", TokenType::WHILE},
        {"do", TokenType::DO},
        //Operators
        {"+", TokenType::PLUS}, {"-", TokenType::MINUS}, {"*", TokenType::STAR}, {"/", TokenType::SLASH}, {"%", TokenType::MODULO}, //'Constant' operators
        {"|", TokenType::OR}, {"&", TokenType::AND}, {"^", TokenType::XOR}, {"~", TokenType::NOT}, //'Constant' bitwise operators
        {"+=", TokenType::PLUS_EQUAL}, {"-=", TokenType::MINUS_EQUAL}, {"*=", TokenType::STAR_EQUAL}, {"/=", TokenType::SLASH_EQUAL}, {"%=", TokenType::MODULO_EQUAL},
        {"|=", TokenType::OR_EQUAL}, {"&=", TokenType::AND_EQUAL}, {"^=", TokenType::XOR_EQUAL}, {"=", TokenType::EQUAL}, //'Nonconstant' operators
        //Boolean operators
        {"!", TokenType::BANG}, {"!", TokenType::BANG_EQUAL}, {"==", TokenType::EQUAL_EQUAL}, {"<", TokenType::LESS}, {"<=", TokenType::LESS_EQUAL},
        {">", TokenType::GREATER}, {">=", TokenType::GREATER_EQUAL}, {"||", TokenType::OROR}, {"&&", TokenType::ANDAND}, {"^^", TokenType::XORXOR}, {",", TokenType::COMMA},
        //Separators
        {";", TokenType::SEMICOLON}, {".", TokenType::DOT}, {":", TokenType::COLON}, {"?", TokenType::COLON},
        //Brackets
        {"(", TokenType::L_PAREN}, {")", TokenType::R_PAREN}, {"{", TokenType::L_BRACE}, {"}", TokenType::R_BRACE}, {"[", TokenType::L_SQUARE_BRACKET}, {"]", TokenType::R_SQUARE_BRACKET},
        //Literals
        {"false", TokenType::BOOLEAN}, {"true", TokenType::BOOLEAN}
    };
    for(const auto &it : stringToTokentype) {
        lexer.addWord(it.first, it.second);
    }
}

};
