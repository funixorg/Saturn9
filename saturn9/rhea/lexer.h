#include <stdbool.h>
#include <tokens.h>

#ifndef LEXER_H
#define LEXER_H


typedef struct LexerStatus {
    Token current_token;
    char current_char;
    unsigned pos;
    unsigned line;
    unsigned column;
    char* content;
} LexerStatus;

void lexer_proc(char* source);
void advance();
void tok_processor();
bool is_number(char ch);
Token numeric_lex();


#endif