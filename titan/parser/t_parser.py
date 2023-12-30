from ..common.token import *
from ..compiler import *

RegNames = {
    "RAX": Registers.RAX,
    "RCX": Registers.RCX,
    "RBX": Registers.RBX,
    "RSP": Registers.RSP,
    "RBP": Registers.RBP,
    "RSI": Registers.RSI,
    "R8" : Registers.R8,
    "R9" : Registers.R9,
    "R10": Registers.R10,
    "R11": Registers.R11,
    "R12": Registers.R12,
    "R13": Registers.R13,
    "R14": Registers.R14,
    "R15": Registers.R15,
}

class Sections:
    DCB   = 0
    DATA  = 1
    TEXT  = 2

class Parser():
    def __init__(self, a_tokens: list[Token]):
        self.a_tokens = a_tokens
        self.exe_tree = Executable()
        self.c_pos    = 0
        self.c_tok    = self.a_tokens[self.c_pos]
        self.t_size   = len(self.a_tokens)
        self.c_sect   = Sections.DCB
        self.c_func   = None
        
    
    def advance(self):
        self.c_pos+=1
        if (self.c_pos < self.t_size):
            self.c_tok = self.a_tokens[self.c_pos]
    
    def eat(self, types:list[TokenType], value:str=None):
        if (value and self.c_token.value != value):
            raise Exception(f"Wrong token value: {self.c_tok.value}")
        if (self.c_tok.type in types):
            tok = self.c_tok
            self.advance()
            return tok
        else:
            raise Exception(f"Wrong token type: {self.c_tok.value}")
        
    
    def process_value(self, value:Token):
        if value.type == TokenType.VARIABLE:
            return Var(value.value)
        elif value.type == TokenType.REGISTER:
            if value.value.upper() in RegNames.keys():
                return RegNames[value.value.upper()]
            else:
                raise Exception(f"Register not found: {value.value}")
        else:
            return value.value
    
    def parse_type(self, value:str):
        if value == "i32":
            return Types.INT32
        elif value == "i8":
            return Types.INT8
        elif value == "addr":
            return Types.POINTER
        elif value == "str":
            return Types.STRING
        else:
            raise Exception(f"Unknown data type: {value}")
        
        
    def process_tokens(self)->Executable:
        while (self.c_pos < self.t_size):
            self.process_token()
        
        if self.c_func:
            self.exe_tree.new_function(
                self.c_func[0],
                self.c_func[1],
                self.c_func[2]
            )

        return self.exe_tree
    
    def process_token(self):
        if self.c_tok.type == TokenType.KEYWORD:
            self.parse_keyword()
        
        elif (self.c_sect == Sections.DATA and self.c_tok.type == TokenType.IDENTIFIER):
            self.parse_variable()
        
        elif (self.c_sect == Sections.TEXT and self.c_tok.type == TokenType.MINUS):
            self.parse_function()
        
        elif (self.c_sect == Sections.TEXT and self.c_func and self.c_tok.type == TokenType.PLUS):
            self.parse_fn_param()
        
        elif (self.c_sect == Sections.TEXT and self.c_func and self.c_tok.value.lower() == "mov"):
            self.parse_mov()
        
        elif (self.c_sect == Sections.TEXT and self.c_func and self.c_tok.value.lower() == "add"):
            self.parse_add()
        
        elif (self.c_sect == Sections.TEXT and self.c_func and self.c_tok.value.lower() == "sub"):
            self.parse_sub()
        
        elif (self.c_sect == Sections.TEXT and self.c_func and self.c_tok.value.lower() == "mul"):
            self.parse_mul()
        
        elif (self.c_sect == Sections.TEXT and self.c_func and self.c_tok.value.lower() == "div"):
            self.parse_div()
        
        elif (self.c_sect == Sections.TEXT and self.c_func and self.c_tok.value.lower() == "ret"):
            self.parse_ret()
        
        elif (self.c_sect == Sections.TEXT and self.c_func and self.c_tok.value.lower() == "push"):
            self.parse_push()
        
        elif (self.c_sect == Sections.TEXT and self.c_func and self.c_tok.value.lower() == "call"):
            self.parse_call()
        
        elif (self.c_sect == Sections.TEXT and self.c_func and self.c_tok.value.lower() == "int"):
            self.parse_int()
        
        else:
            raise Exception(f"Unknown operation: {self.c_tok.value}")
            
    
    def parse_keyword(self):
        if (self.c_tok.value == "section"):
            self.parse_section()
        elif (self.c_tok.value == "define"):
            self.parse_definition()
        
    
    def parse_section(self):
        self.eat([TokenType.KEYWORD])
        sect = self.eat([TokenType.IDENTIFIER]).value
        match sect.lower():
            case "dcb":
                self.c_sect = Sections.DCB
            case "data":
                self.c_sect = Sections.DATA
            case "text":
                self.c_sect = Sections.TEXT
        self.eat([TokenType.COLON])

    
    def parse_definition(self):
        self.eat([TokenType.KEYWORD]) 
        c_name = self.eat([TokenType.IDENTIFIER]).value
        c_value = self.eat([TokenType.INTEGER, TokenType.STRING])
        if (self.c_sect != Sections.DCB):
            raise Exception("Constants are allowed only in DCB")

        self.exe_tree.new_constant(
            c_name,
            self.process_value(c_value)
        )
    
    def parse_variable(self):
        v_type  = self.parse_type(self.eat([TokenType.IDENTIFIER]).value)
        v_name  = self.eat([TokenType.IDENTIFIER]).value
        v_value = None
        if (self.c_tok.type == TokenType.EQUAL):
            self.advance()
            v_value = self.process_value(
                self.eat([TokenType.INTEGER, TokenType.STRING, TokenType.VARIABLE])
            )
        
        self.exe_tree.new_variable(
            v_name, v_value, v_type
        )
    
    def parse_function(self):
        if self.c_func:
            self.exe_tree.new_function(
                self.c_func[0],
                self.c_func[1],
                self.c_func[2]
            )
        
        self.eat([TokenType.MINUS])
        f_name = self.eat([TokenType.IDENTIFIER]).value
        self.eat([TokenType.COLON])
        
        self.c_func = [f_name, [], []]
    
    def parse_fn_param(self):
        self.eat([TokenType.PLUS])
        p_type = self.parse_type(self.eat([TokenType.IDENTIFIER]).value)
        p_name = self.eat([TokenType.IDENTIFIER]).value
        self.c_func[1].append((p_name, p_type))
    
    def parse_mov(self):
        self.eat([TokenType.IDENTIFIER])
        target = self.process_value(
                self.eat([TokenType.INTEGER, TokenType.STRING, TokenType.VARIABLE, TokenType.REGISTER])
            )
        self.eat([TokenType.COMMA])
        source = self.process_value(
                self.eat([TokenType.INTEGER, TokenType.STRING, TokenType.VARIABLE, TokenType.REGISTER])
            )

        self.c_func[2].append(
            self.exe_tree.Move(target, source)
        )
    
    def parse_add(self):
        self.eat([TokenType.IDENTIFIER])
        target = self.process_value(
                self.eat([TokenType.INTEGER, TokenType.STRING, TokenType.VARIABLE, TokenType.REGISTER])
            )
        self.eat([TokenType.COMMA])
        source = self.process_value(
                self.eat([TokenType.INTEGER, TokenType.STRING, TokenType.VARIABLE, TokenType.REGISTER])
            )

        self.c_func[2].append(
            self.exe_tree.Add(target, source)
        )
    
    def parse_sub(self):
        self.eat([TokenType.IDENTIFIER])
        target = self.process_value(
                self.eat([TokenType.INTEGER, TokenType.STRING, TokenType.VARIABLE, TokenType.REGISTER])
            )
        self.eat([TokenType.COMMA])
        source = self.process_value(
                self.eat([TokenType.INTEGER, TokenType.STRING, TokenType.VARIABLE, TokenType.REGISTER])
            )

        self.c_func[2].append(
            self.exe_tree.Sub(target, source)
        )
    
    def parse_mul(self):
        self.eat([TokenType.IDENTIFIER])
        target = self.process_value(
                self.eat([TokenType.INTEGER, TokenType.STRING, TokenType.VARIABLE, TokenType.REGISTER])
            )
        self.eat([TokenType.COMMA])
        source = self.process_value(
                self.eat([TokenType.INTEGER, TokenType.STRING, TokenType.VARIABLE, TokenType.REGISTER])
            )

        self.c_func[2].append(
            self.exe_tree.Mul(target, source)
        )
    
    def parse_div(self):
        self.eat([TokenType.IDENTIFIER])
        target = self.process_value(
                self.eat([TokenType.INTEGER, TokenType.STRING, TokenType.VARIABLE, TokenType.REGISTER])
            )
        self.eat([TokenType.COMMA])
        source = self.process_value(
                self.eat([TokenType.INTEGER, TokenType.STRING, TokenType.VARIABLE, TokenType.REGISTER])
            )

        self.c_func[2].append(
            self.exe_tree.Div(target, source)
        )
    
    def parse_ret(self):
        self.eat([TokenType.IDENTIFIER])
        self.c_func[2].append(
            self.exe_tree.Return()
        )
    
    def parse_push(self):
        self.eat([TokenType.IDENTIFIER])
        target = self.process_value(
                self.eat([TokenType.INTEGER, TokenType.STRING, TokenType.VARIABLE, TokenType.REGISTER])
            )

        self.c_func[2].append(
            self.exe_tree.Push(target)
        )
    
    def parse_call(self):
        self.eat([TokenType.IDENTIFIER])
        fn_name = self.process_value(
                self.eat([TokenType.IDENTIFIER])
            )

        self.c_func[2].append(
            self.exe_tree.Call(fn_name)
        )
    
    def parse_int(self):
        self.eat([TokenType.IDENTIFIER])
        self.c_func[2].append(
            self.exe_tree.Interrupt()
        )

def parser_conv(tokens: list[Token])->Executable:
    parser = Parser(tokens)
    execv = parser.process_tokens()
    
    return execv