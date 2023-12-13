#include <stdbool.h>
#include <tokens.h>

#ifndef LEXER_H
#define LEXER_H


typedef struct LexerStatus {
    char current_char;
    unsigned pos;
    unsigned line;
    unsigned column;
    char* content;
    unsigned size;
} LexerStatus;

Token **lexer_proc(char* source);
void l_advance();
Token *tok_processor();
bool is_number(char ch);
bool is_alpha(char ch);
bool is_hex(char ch);
bool is_symbol(char ch);
enum TokenType symeqtok(char ch);
Token *numeric_lex();
Token *identifier_lex();
Token *string_lex();
Token *symbol_lex();

#endif