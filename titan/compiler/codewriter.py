from .opcodes import *
from .dtype import *
from .executable import *
import struct
from sys import getsizeof as sizeof

class mcWriter():
    def __init__(self, executable: Executable, path:str):
        self.execs = executable
        self.file  = open(path, "wb")
        self.file.write(
            struct.pack("<i",TITAN_SIGNATURE) # Executable sign
        )
        self.file.write(
            struct.pack("<i",0) # DCB Size
        )
        self.file.write(
            struct.pack("<i",0) # Data Size
        )
        self.file.write(
            struct.pack("<i",0) # Text Size
        )
        
        self.dcb  = 0
        self.data = 0
        self.text = 0
        
        self.process()
        self.file.seek(4,0) # 4 to Skip Sign
        self.file.write(
            struct.pack("<i",self.dcb) # DCB Size 
        )
        self.file.write(
            struct.pack("<i",self.data) # Data Size
        )
        self.file.write(
            struct.pack("<i",self.text) # Data Size
        )
        self.file.close()
        
    def process(self)->None:
        for obj in self.execs.dcb:
            if not isinstance(obj, self.execs.Constant):
                raise Exception(f"{type(obj).__name__} Not Allowed in DCB!")
            
            name   = obj.name
            value  = obj.value
            machine_code = struct.pack(
                f"<h{len(name)}s", len(name), name.encode()
            ) + self.process_value(value)
            self.file.write(machine_code)
            self.dcb+=1
        
        for obj in self.execs.data:
            if not isinstance(obj, self.execs.Variable):
                raise Exception(f"{type(obj).__name__} Not Allowed in Data!")
            
            name   = obj.name
            value  = (obj.value if obj.value else 0)
            machine_code = struct.pack(
                f"<h{len(name)}s", len(name), name.encode()
            ) + self.process_value(value)
            self.file.write(machine_code)
            self.data+=1
        
        for obj in self.execs.text:
            if not isinstance(obj, self.execs.Function):
                raise Exception(f"{type(obj).__name__} Not Allowed in Text!")

            name       = obj.name
            parameters = obj.parameters
            body       = obj.body
            
            paramsfmt = b""
            for param in parameters:
                paramsfmt += struct.pack(
                    f"<h{len(param[0])}s", len(param[0]), (param[0]).encode()
                )
            
            machine_code = struct.pack(
                f"<ih{len(name)}shh", FUNCTION_SIGNATURE, len(name), name.encode(),
                len(parameters), len(body)
            ) + paramsfmt
            self.file.write(machine_code)
            
            for instruction in body:
                inscode = self.process_instruction(instruction)
                print(instruction, inscode)
                self.file.write(
                    inscode
                )
            self.text+=1
    
    def process_value(self, value:any):
        o_type = ret_type(value)
        if o_type == Types.IDENTIFIER:
            target_code = struct.pack(
                f"<hh{len(value.variable)}s", o_type.id, len(value.variable), value.variable.encode()
            )
        elif o_type == Types.STRING:
            target_code = struct.pack(
                f"<hh{len(value)}s", o_type.id, len(value), value.encode()
            )
        elif o_type == Types.POINTER:
            target_code = struct.pack(
                f"<h"+o_type.format, o_type.id, value.address
            )
        elif o_type == Types.REGISTER:
            target_code = struct.pack(
                f"<h"+o_type.format, o_type.id, value.opcode
            )
        else:
            target_code = struct.pack(
                "<h"+o_type.format, o_type.id, value
            )
        return target_code
        
    def process_instruction(self, instruction:any):
        if isinstance(instruction, self.execs.Move):
            target   = instruction.target
            source   = instruction.source
            target_o = self.process_value(target)
            source_o = self.process_value(source)
            return struct.pack("<h", Instructions.MOV.opcode) + target_o + source_o

        elif isinstance(instruction, self.execs.Call):
            name = instruction.name
            return struct.pack(f"<hh{len(name)}s", Instructions.CALL.opcode, len(name), name.encode())
        
        elif isinstance(instruction, self.execs.Push):
            value   = instruction.value
            value_o = self.process_value(value)
            return struct.pack("<h", Instructions.PUSH.opcode) + value_o

        elif isinstance(instruction, self.execs.Return):
            return struct.pack("<h", Instructions.RET.opcode)

        elif isinstance(instruction, self.execs.Interrupt):
            return struct.pack("<h", Instructions.INT.opcode)
        
        elif isinstance(instruction, self.execs.Lea):
            target   = instruction.target
            source   = instruction.source
            target_o = self.process_value(target)
            source_o = self.process_value(source)
            return struct.pack("<h", Instructions.LEA.opcode) + target_o + source_o
        
        elif isinstance(instruction, self.execs.Add):
            target   = instruction.target
            source   = instruction.source
            target_o = self.process_value(target)
            source_o = self.process_value(source)
            return struct.pack("<h", Instructions.ADD.opcode) + target_o + source_o
        
        elif isinstance(instruction, self.execs.Sub):
            target   = instruction.target
            source   = instruction.source
            target_o = self.process_value(target)
            source_o = self.process_value(source)
            return struct.pack("<h", Instructions.SUB.opcode) + target_o + source_o
        
        elif isinstance(instruction, self.execs.Mul):
            target   = instruction.target
            source   = instruction.source
            target_o = self.process_value(target)
            source_o = self.process_value(source)
            return struct.pack("<h", Instructions.MUL.opcode) + target_o + source_o
        
        elif isinstance(instruction, self.execs.Div):
            target   = instruction.target
            source   = instruction.source
            target_o = self.process_value(target)
            source_o = self.process_value(source)
            return struct.pack("<h", Instructions.DIV.opcode) + target_o + source_o