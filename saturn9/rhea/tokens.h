#ifndef TOKENS_H
#define TOKENS_H

enum TokenType {
    INTEGER,
    HEXINT,
    IDENTIFIER,
    STRING,


    PLUS,
    MINUS,
    MUL,
    DIV,
    EQUAL,
    NOT,
    GREATER,
    SMALLER,


    LPAREN,
    RPAREN,
    
    AT,
    COMMA,
    COLON,
    SEMICOLON,

    LBRACE,
    RBRACE,
    LBRACKET,
    RBRACKET,


    EOS, // End of token stream,
    WS,
    ERR
};

typedef struct Token {
    enum TokenType type;
    char* value;
} Token;


typedef struct SymbolType {
    char symbol;
    enum TokenType type;
} SymbolType;

#endif