from ringprot.define import Definition, Sizes
from struct import pack as _upack

class Section:
    def __init__(self, name:str):
        self.name:str      = name
        self.content:bytes = b""


class ProgramHeader:
    def __init__(self):
        pass
    
    def __sizeof__() -> int:
        return (
            0
        )
    
class SectionHeader:
    def __init__(self):
        pass

class Header:
    def __init__(self):
        self.size:bytes    = Header.__sizeof__()         # 0x00
        self.magic:bytes   = Definition.MAGIC            # 0x06
        self.arch:bytes    = None                        # 0x0A
        self.version:bytes = Definition.Version.current  # 0x0B
        self.order:bytes   = None                        # 0x0C
        self.entry:int     = 0x00                        # 0x10
        self.phoff         = None                        # 0x14
        self.shoff         = None                        # 0x18
    
    def __sizeof__() -> int:
        return (
            0
            +Sizes.INTEGER8
            +len(Definition.MAGIC)
            +Definition.Architecture.__sizeof__()
            +Definition.Version.current.__sizeof__()
            +Definition.Order.little.__sizeof__()
            +Sizes.INTEGER32
            +Sizes.INTEGER32
            +Sizes.INTEGER32
        )


class Writer:
    def __init__(self):
        self.name:str                  = "tmpexec"
        self.outbuffer:bytes           = b""
        self.sections:list[Section]    = []
        self.header:bytes              = None
        self.sectheader:SectionHeader  = None