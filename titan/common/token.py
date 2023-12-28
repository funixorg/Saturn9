

class TokenType:
    IDENTIFIER  = 0
    INTEGER     = 1
    STRING      = 2
    
    LBRACKET    = 3
    RBRACKET    = 4
    MINUS       = 5
    PLUS        = 6
    MOD         = 7
    DOLLAR      = 8
    COMMA       = 9
    COLON       = 10
    EQUAL       = 11
    
    KEYWORD     = 12
    REGISTER    = 13
    VARIABLE    = 14


class Token():
    def __init__(self, token_type: TokenType, token_value: any):
        self.type  = token_type
        self.value = token_value