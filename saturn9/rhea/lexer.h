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
Token tok_processor();
bool is_number(char ch);
bool is_alpha(char ch);
bool is_hex(char ch);
bool is_symbol(char ch);
Token numeric_lex();
Token identifier_lex();
Token string_lex();

#endif