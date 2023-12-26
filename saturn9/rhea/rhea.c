#include <rhea.h>
#include <lexer.h>
#include <parser.h>
#include <tokens.h>


void rhea_proc(char* source) {
    Token **tokens = lexer_proc(source);
    parser_proc(tokens);
}