#include <lexer.h>
#include <tokens.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <mem.h>

SymbolType symbols[] = {
    {'+', PLUS},
    {'-', MINUS},
    {'*', MUL},
    {'/', DIV},
    {'(', LPAREN},
    {')', RPAREN},
    {'@', AT},
    {',', COLON},
    {';', SEMICOLON},
    {'{', LBRACE},
    {'}', RBRACE},
    {'[', LBRACKET},
    {']', RBRACKET},
    {'=', EQUAL},
    {'!', NOT},
    {'>', GREATER},
    {'<', SMALLER},
    
};

LexerStatus lexstat;

Token **lexer_proc(char *source) {
    lexstat.content = source;
    lexstat.pos = 0;
    lexstat.line = 1;
    lexstat.column = 1;
    lexstat.size = strlen(source);
    lexstat.current_char = source[lexstat.pos];
    printf_serial("Ready, Lexer, Go\n");

    Token **tokens = memalloc((lexstat.size + 1) * sizeof(Token*));
    Token *ctok;
    unsigned t_index = 0;

    while (lexstat.pos < lexstat.size) {
        ctok = tok_processor();
        if (ctok->type != ERR && ctok->type != WS) {
            tokens[t_index] = memalloc(sizeof(Token));
            *(tokens[t_index]) = *ctok;
            t_index++;
        }
    }

    Token *eos = memalloc(sizeof(Token));
    eos->type = EOS;
    eos->value = memalloc(1);
    eos->value[0] = '\0';
    tokens[t_index] = eos;

    return tokens;
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


bool is_symbol(char ch) {
    for (unsigned _i=0;_i<sizeof(symbols)/sizeof(SymbolType);_i++) {
        if (ch==symbols[_i].symbol) {
            return true;
        }
    }
    return false;
}


void advance() {
    lexstat.pos++;
    if (lexstat.pos < lexstat.size) {
        if (lexstat.current_char == '\n') {
            lexstat.line++;
            lexstat.column = 1;
        } else {
            lexstat.column++;
        }

        lexstat.current_char = lexstat.content[lexstat.pos];
    } else {
        lexstat.current_char = '\0';
    }
}


Token *tok_processor() {
    if (is_number(lexstat.current_char)) {
        return numeric_lex();
    }
    else if (lexstat.current_char == '"') {
        return string_lex();
    }
    else if (is_alpha(lexstat.current_char)) {
        return identifier_lex();
    }
    else if (is_symbol(lexstat.current_char)) {
        return symbol_lex();
    }
    else {
        if (lexstat.current_char == ' ' || lexstat.current_char == '\t' || lexstat.current_char == '\n' || lexstat.current_char == 0x1A) {
            advance();
        }
        else {
            printf_serial("Unrecognized character: `%c` at %d (ascii: %d)\n", lexstat.current_char, lexstat.pos, (int)lexstat.current_char);
            advance();
            Token *terr=NULL;
            terr = memalloc(sizeof(Token));
            terr->type=ERR;
            terr->value="\0";
            return terr;
        }
    }
    Token *tws=NULL;
    tws = memalloc(sizeof(Token));
    tws->type=WS;
    tws->value=" ";
    return tws;
}

enum TokenType symeqtok(char ch) {
    for (unsigned _i=0;_i<sizeof(symbols)/sizeof(SymbolType);_i++) {
        if (ch==symbols[_i].symbol) {
            return symbols[_i].type;
        }
    }
    return PLUS;
}

Token *numeric_lex() {
    char *value=memalloc(64);
    unsigned _i=0;
    int bis_hex=0;
    while (lexstat.pos < lexstat.size && (is_number(lexstat.current_char) || is_hex(lexstat.current_char))) {
        if ( is_hex(lexstat.current_char) ) { bis_hex=1; }
        value[_i]=lexstat.current_char;
        advance();
        _i++;

    }
    value[_i]='\0';

    Token *ntok = NULL;
    ntok = memalloc(sizeof(Token));
    if (!bis_hex) { ntok->type = INTEGER; }
    else { ntok->type = HEXINT; }
    ntok->value = value;
    printf_serial("INT/HEX: %s\n", value);
    return ntok;
}


Token *identifier_lex() {
    char *value=memalloc(256);
    unsigned _i=0;
    while (lexstat.pos < lexstat.size && is_alpha(lexstat.current_char)) {
        value[_i]=lexstat.current_char;
        advance();
        _i++;
    }
    value[_i]='\0';
    printf_serial("IDENTIFIER: %s\n", value);
    Token *itok=NULL;
    itok = memalloc(sizeof(Token));
    itok->type=IDENTIFIER;
    itok->value=value;
    return itok;
}

Token *string_lex() {
    char *value=memalloc(256);
    unsigned _i=0;
    advance(); // Skip quote
    while (lexstat.pos < lexstat.size && lexstat.current_char != '"') {
        value[_i]=lexstat.current_char;
        advance();
        _i++;
    }
    advance(); // Skip quote
    value[_i]='\0';
    printf_serial("STRING: \"%s\"\n", value);

    Token *stok=NULL;
    stok = memalloc(sizeof(Token));
    stok->type=STRING;
    stok->value=value;
    return stok;
}


Token *symbol_lex() {
    Token *stok=NULL;
    stok = memalloc(sizeof(Token));
    // Someway the older way to do this was replacing the last source char with the first lexed 
    // IDK HOW TO FIX THIS SO THIS IS JUST A WORKAROUND
    // the workaround is being done by allocating double the size of the source

    stok->value=memalloc(2);
    stok->value[0]=lexstat.current_char;
    stok->value[1]='\0';
    stok->type=symeqtok(lexstat.current_char);
    advance();
    printf_serial("SYMBOL: %s\n", stok->value);
    return stok;
}
