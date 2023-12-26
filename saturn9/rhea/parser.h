#include <tokens.h>

#ifndef PARSER_H
#define PARSER_H

typedef struct ParserStatus {
    Token *current_token;
    unsigned pos;
    Token **tokens;
} ParserStatus;


void parser_proc(Token **tokens);


#endif