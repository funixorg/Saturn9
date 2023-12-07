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


    LPAREN,
    RPAREN
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