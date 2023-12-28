from .opcodes import *

class Char:
    def __init__(self, ch:str):
        self.char = ch

class Pointer():
    def __init__(self, obj:any):
        self.address = obj.address

class Var():
    def __init__(self, var:str):
        self.variable = var

class DataType:
    def __init__(self, size:int, id:int, fmt:str):
        self.size   = size
        self.id     = id
        self.format = fmt

class Types():
    INT32      = DataType(4,   0,  "i")
    INT8       = DataType(1,   1,  "b")
    POINTER    = DataType(8,   2,  "P")
    REGISTER   = DataType(2,   3,  "h")
    IDENTIFIER = DataType(0,   4,  "h")
    STRING     = DataType(0,   5,  "h")

def ret_type(obj:any):
    if isinstance(obj, Pointer):
        return Types.POINTER
    elif isinstance(obj, Register):
        return Types.REGISTER
    elif isinstance(obj, int):
        return Types.INT32
    elif isinstance(obj, Char):
        return Types.INT8
    elif isinstance(obj, Var):
        return Types.IDENTIFIER
    elif isinstance(obj, str):
        return Types.STRING
    else:
        raise Exception(f"{type(obj).__name__} Unsupported Type!")