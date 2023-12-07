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
    Token tokens[4028];
    unsigned t_index=0;
    while (lexstat.pos < strlen(lexstat.content)) {
        tokens[t_index]=tok_processor();
        t_index++;
    }
}

void advance() {
    if (lexstat.pos < strlen(lexstat.content)) {
        lexstat.pos++;
        lexstat.current_char = lexstat.content[lexstat.pos];
    }
}

Token tok_processor() {
    if (is_number(lexstat.current_char)) {
        return numeric_lex();
    }
    else if (is_alpha(lexstat.current_char)) {
        return identifier_lex();
    }
    else if (lexstat.current_char == '"') {
        return string_lex();
    }
    else {advance();}
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

bool is_alpha(char ch) {
    if (
        (ch == '0' || ch == '1' || ch == '2' || ch == '3' || ch == '4' || ch == '5' ||
        ch == '6' || ch == '7' || ch == '8' || ch == '9') || (ch >= 'A' && ch <= 'z') || (ch == '.') || (ch=='_')
    ) {
        return true;
    }
    return false;
}


bool is_hex(char ch) {
    if ((
    ch=='x'  || ch=='a' ||
    ch=='b'  || ch=='c' ||
    ch=='d'  || ch=='e' ||
    ch=='f') ||
    
    (ch=='A' || ch=='B' ||
    ch=='C'  || ch=='D' ||
    ch=='E'  || ch=='F')) {
        return true;
    }
    return false;
}

Token numeric_lex() {
    char *value=memalloc(64);
    unsigned _i=0;
    int bis_hex=0;
    while (lexstat.pos < strlen(lexstat.content) && (is_number(lexstat.current_char) || is_hex(lexstat.current_char))) {
        if ( is_hex(lexstat.current_char) ) { bis_hex=1; }
        value[_i]=lexstat.current_char;
        advance();
        _i++;

    }
    value[_i]='\0';

    Token ntok;
    if (!bis_hex) { ntok.type = INTEGER; }
    else { ntok.type = HEXINT; }
    ntok.value = value;
    printf_serial("INT/HEX: %s\n", value);
    return ntok;
}


Token identifier_lex() {
    char *value=memalloc(256);
    unsigned _i=0;
    while (lexstat.pos < strlen(lexstat.content) && is_alpha(lexstat.current_char)) {
        value[_i]=lexstat.current_char;
        advance();
        _i++;
    }
    printf_serial("IDENTIFIER: %s\n", value);
    Token itok;
    itok.type=IDENTIFIER;
    itok.value=value;
    return itok;
}

Token string_lex() {
    char *value=memalloc(256);
    unsigned _i=0;
    advance(); // Skip quote
    while (lexstat.pos < strlen(lexstat.content) && lexstat.current_char != '"') {
        value[_i]=lexstat.current_char;
        advance();
        _i++;
    }
    advance();
    printf_serial("STRING: \"%s\"\n", value);

    Token stok;
    stok.type=STRING;
    stok.value=value;
    return stok;
}