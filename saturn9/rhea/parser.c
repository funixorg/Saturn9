#include <parser.h>
#include <tokens.h>

ParserStatus parserstat;


void p_advance() {
    parserstat.pos++;
    parserstat.current_token = parserstat.tokens[parserstat.pos];
}

void parser_proc(Token **tokens) {
    parserstat.tokens=tokens;
    parserstat.pos=0;
    parserstat.current_token=parserstat.tokens[parserstat.pos];

    while (parserstat.current_token->type!=EOS) {
        p_advance();
    }
}


void node_processor() {

}