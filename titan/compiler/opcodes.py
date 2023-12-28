class Register:
    def __init__(self, reg:int):
        self.opcode = reg
        
class Instruction:
    def __init__(self, ins:int):
        self.opcode = ins

class Instructions():
    MOV  = Instruction(0x00)
    CALL = Instruction(0x01)
    PUSH = Instruction(0x02)
    RET  = Instruction(0x03)
    LEA  = Instruction(0x04)
    
    ADD  = Instruction(0x05)
    SUB  = Instruction(0x06)
    MUL  = Instruction(0x07)
    DIV  = Instruction(0x08)
    
    INT  = Instruction(0x09)

class Registers():
    RAX  = Register(0x00)
    RCX  = Register(0x01)
    RDX  = Register(0x02)
    
    RBX  = Register(0x03)
    RSP  = Register(0x04)
    RBP  = Register(0x05)
    
    RSI  = Register(0x06)
    RDI  = Register(0x07)
    
    R8   = Register(0x08)
    R9   = Register(0x09)
    R10  = Register(0x0A)
    R11  = Register(0x0B)
    R12  = Register(0x0C)
    R13  = Register(0x0D)
    R14  = Register(0x0E)
    R15  = Register(0x0F)

TITAN_SIGNATURE = 0x18A
FUNCTION_SIGNATURE = 0xF0