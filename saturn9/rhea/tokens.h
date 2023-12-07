#ifndef TOKENS_H
#define TOKENS_H

enum TokenType {
    INTEGER,
    HEXINT,
    IDENTIFIER,
    STRING
};

typedef struct Token {
    enum TokenType type;
    char* value;
} Token;

#endif