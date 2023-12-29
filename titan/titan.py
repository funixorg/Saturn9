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
    compile_tas("titan/sysp/pan.tas", "initramdir/sys/bin/pan")
    
if __name__ == "__main__": titan_gen()