#include <lexer.h>
#include <tokens.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <mem.h>

LexerStatus lexstat;

void lexer_proc(char* source) {
    lexstat.pos = 0;
    lexstat.line=1;
    lexstat.content = source;
    lexstat.column=1;
    lexstat.current_char = source[lexstat.pos];
    printf_serial("Ready, Lexer, Go\n");
    while (lexstat.pos < sizeof(source)/sizeof(char*)) {
        tok_processor();
    }
}

void advance() {
    if (lexstat.pos < sizeof(lexstat.content)/sizeof(char*)) {
        lexstat.pos++;
        lexstat.current_char = lexstat.content[lexstat.pos];
    }
}

void tok_processor() {
    if (is_number(lexstat.current_char)) {
        numeric_lex();
        return;
    }
    advance();
}


bool is_number(char ch) {
    if (
        ch == '0' || ch == '1' || ch == '2' || ch == '3' || ch == '4' || ch == '5' ||
        ch == '6' || ch == '7' || ch == '8' || ch == '9'
    ) {
        return true;
    }
    return false;
}

Token numeric_lex() {
    char *value=memalloc(64);
    unsigned _i=0;
    while (is_number(lexstat.current_char)) {
        value[_i]=lexstat.current_char;
        advance();
        _i++;
    }
    value[_i]='\0';
    
    Token ntok;
    ntok.type = INTEGER;
    ntok.value = value;
    return ntok;
}