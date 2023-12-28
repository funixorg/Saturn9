"""
from .compiler.opcodes import *
from .compiler.dtype import *
from .compiler.codewriter import *
from .compiler.executable import *


test_prog = Executable()
test_prog.new_constant(
    "magic",
    0xF1A
)
test_prog.new_variable(
    "offset",
    None,
    Types.INT32
)
test_prog.new_variable(
    "name",
    "Unity",
    Types.STRING
)
test_prog.new_function(
    "add",
    [("a", Types.INT32),
    ("b", Types.INT32)],
    [
        test_prog.Move(Registers.RAX, Var("a")),
        test_prog.Move(Registers.RCX, Var("b")),
        test_prog.Add(Registers.RAX, Registers.RCX),
        test_prog.Return()
    ]
)
test_prog.new_function(
    "main",
    [],
    [
        #test_prog.Move(Registers.RAX, 5),
        #test_prog.Add(Registers.RAX, 4),
        #test_prog.Mul(Registers.RAX, 2),
        #test_prog.Move(Var("offset"), 444),
        #test_prog.Move(Registers.RAX, Var("offset")),
        #test_prog.Push(1),
        #test_prog.Push(2),
        #test_prog.Call("add"),
        
        #test_prog.Move(Registers.RAX, 0x90), 
        
        #test_prog.Interrupt(),
        
        test_prog.Move(Registers.RAX, 0x91), 
        test_prog.Move(Registers.RBX,"#{0x00ebeb}Name > \\@RES"), 
        test_prog.Interrupt(),
        
        test_prog.Move(Registers.RAX, 0x92),
        test_prog.Interrupt(),
        
        test_prog.Move(Var("name"), Registers.RSI),
        
        test_prog.Move(Registers.RAX, 0x91),
        test_prog.Move(Registers.RBX,"#{0xc43b3b}Name: "), 
        test_prog.Interrupt(),
        
        test_prog.Move(Registers.RBX,Var("name")), 
        test_prog.Interrupt(),
        
        test_prog.Move(Registers.RBX,"\n"), 
        test_prog.Interrupt()
    ]
)


clear_prog = Executable()
clear_prog.new_function(
    "main",
    [],
    [
        clear_prog.Move(Registers.RAX, 0x90),
        clear_prog.Interrupt()
    ]
)



def titan_gen():
    mcWriter(test_prog, "initramdir/sys/bin/testbin")
    mcWriter(clear_prog, "initramdir/sys/bin/clear")
"""
from .lexer import *
from .parser import *
from .compiler import *

def compile_tas(path:str, output:str):
    tokens = lexerize(path)
    execv  = parser_conv(tokens)
    mcWriter(execv, output)

def titan_gen():
    compile_tas("titan/sysp/test.tas", "initramdir/sys/bin/testbin")
    compile_tas("titan/sysp/clear.tas", "initramdir/sys/bin/clear")
    
if __name__ == "__main__": titan_gen()