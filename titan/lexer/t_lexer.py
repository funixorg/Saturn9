from ..common.token import *
import string

SYMBOLS_MAP = [
    (",", TokenType.COMMA),
    ("[", TokenType.LBRACKET),
    ("]", TokenType.RBRACKET),
    ("-", TokenType.MINUS),
    ("+", TokenType.PLUS),
    ("%", TokenType.MOD),
    ("$", TokenType.DOLLAR),
    (":", TokenType.COLON),
    ("=", TokenType.EQUAL)
]


class Utils:
    def is_hex(s):
        hex_digits = set(string.hexdigits)
        return all(c in hex_digits for c in s)

    def ret_symbol(s) -> int:
        for symbol, token_type in SYMBOLS_MAP:
            if symbol == s:
                return token_type
        return False

class Lexer():
    def __init__(self, source: str):
        self.content = source
        self.c_pos   = 0
        self.col     = 1
        self.line    = 1
        self.c_char  = self.content[self.c_pos]
        self.s_size  = len(self.content)
        self.a_token = []
    
    def advance(self)->None:
        self.c_pos+=1
        if (self.c_pos < self.s_size):
            self.c_char = self.content[self.c_pos]
            self.col  += 1
        

    
    def process_source(self)->list[Token]:
        while (self.c_pos < self.s_size):
            tok = self.process_token()
            if (tok): self.a_token.append(tok)
        return self.a_token
    
    def process_token(self)->Token:
        token: Token = None
        
        if self.c_char.isdigit():
            token = self.process_integer()
        elif self.c_char.isalnum():
            token = self.process_identifier()
        elif self.c_char == '"':
            token = self.process_string()
        elif self.c_char == "[":
            token = self.process_keyword()
        elif self.c_char == '%':
            token = self.process_register()
        elif self.c_char == '$':
            token = self.process_variable()
        elif Utils.ret_symbol(self.c_char):
            token = self.process_symbol()
        elif self.c_char == " ":
            self.advance()
        elif self.c_char == "\n":
            self.line+=1
            self.col=0
            self.advance()
        else:
            raise Exception(f"Unknown character type: {repr(self.c_char)}")
        
        return token
            
    def process_integer(self)->Token:
        t_value = ""
        t_type  = TokenType.INTEGER
        base    = 10
        while ((Utils.is_hex(self.c_char) or self.c_char == 'x') and self.c_pos < self.s_size):
            if self.c_char == 'x': base = 16
            t_value += self.c_char
            self.advance()

        return Token(t_type, int(t_value, base))

    def process_identifier(self)->Token:
        t_value = ""
        t_type  = TokenType.IDENTIFIER
        while (self.c_char.isalnum() and self.c_pos < self.s_size):
            t_value += self.c_char
            self.advance()
        
        return Token(t_type, str(t_value))

    def process_string(self)->Token:
        t_value = ""
        t_type  = TokenType.STRING
        self.advance()
        
        while (self.c_char != '"' and self.c_pos < self.s_size):
            t_value += self.c_char
            self.advance()
        self.advance()
        return Token(t_type, str(t_value).replace("\\n", "\n"))

    def process_symbol(self)->Token:
        t_value = self.c_char
        t_type = Utils.ret_symbol(self.c_char)
        self.advance()
        return Token(t_type, str(t_value))

    def process_keyword(self)->Token:
        t_value = ""
        t_type = TokenType.KEYWORD
        self.advance()
        while (self.c_char != ']' and self.c_pos < self.s_size):
            t_value += self.c_char
            self.advance()
        self.advance()
        return Token(t_type, str(t_value))

    def process_register(self)->Token:
        t_value = ""
        t_type = TokenType.REGISTER
        self.advance()
        while (self.c_char.isalnum() and self.c_pos < self.s_size):
            t_value += self.c_char
            self.advance()
        
        return Token(t_type, str(t_value))

    def process_variable(self)->Token:
        t_value = ""
        t_type = TokenType.VARIABLE
        self.advance()
        while (self.c_char.isalnum() and self.c_pos < self.s_size):
            t_value += self.c_char
            self.advance()
        
        return Token(t_type, str(t_value))
        

def lexerize(path:str):
    source = open(path, "r").read()
    lexer  = Lexer(source)
    return lexer.process_source()