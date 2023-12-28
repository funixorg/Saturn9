from .opcodes import *
from .dtype import *
from sys import getsizeof as sizeof

class Executable():
    def __init__(self):
        self.dcb     = []     # Defined Constants Block
        self.data    = []     # Data Block
        self.text    = []     # Text Block
        self.address = 0x0000 # Lowest memory address
    
    class Constant():
        def __init__(self, name:str, value:any, address:int):
            self.name    = name
            self.value   = value
            self.address = address
    
    class Variable():
        def __init__(self, name:str, value:any, datatype:DataType, address:int):
            self.name    = name
            self.value   = value
            self.type    = datatype
            self.address = address
    
    class Function():
        def __init__(self, name, parameters:list[str], body:any, address:int):
            self.name       = name
            self.parameters = parameters
            self.body       = body
            self.address    = address 
    
    class Move():
        def __init__(self, t_obj:any, s_obj:any):
            self.target = t_obj
            self.source = s_obj
    
    class Call():
        def __init__(self, fn_name:str):
            self.name = fn_name
    
    class Push():
        def __init__(self, value:any):
            self.value = value
    
    class Return():
        def __init__(self):
            pass
    
    class Lea():
        def __init__(self, t_obj:any, s_obj:any):
            self.target = t_obj
            self.source = s_obj
    
    class Add():
        def __init__(self, t_value:any, s_value:any):
            self.target = t_value
            self.source = s_value
            
    class Sub():
        def __init__(self, t_value:any, s_value:any):
            self.target = t_value
            self.source = s_value
            
    class Mul():
        def __init__(self, t_value:any, s_value:any):
            self.target = t_value
            self.source = s_value
            
    class Div():
        def __init__(self, t_value:any, s_value:any):
            self.target = t_value
            self.source = s_value
            
    class Interrupt():
        def __init__(self):
            pass
    
    def new_constant(self, name:str, value:any):
        self.dcb.append(
            self.Constant(name, value, self.address)
        )
        self.address+=ret_type(value).size

    def new_variable(self, name:str, value:any, datatype:DataType):
        self.data.append(
            self.Variable(name, value, datatype, self.address)
        )
        self.address+=datatype.size
    
    def new_function(self, name:str, parameters:list[(str, DataType)], body:list[any])->list:
        self.text.append(
            self.Function(name, parameters, body, self.address)
        )
        self.address+=0x01
        for parameter in parameters:
            self.new_variable(parameter[0], None, parameter[1])