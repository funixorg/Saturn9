from assembler import tasm


def test():
    header         = tasm.Header()
    header.arch    = tasm.Definition.Architecture.x86_64
    header.version = tasm.Definition.Version.current
    header.order   = tasm.Definition.Order.little
    header.entry   = 0x00
    header.phoff   = tasm.Header.__sizeof__()
    header.shoff   = tasm.Header.__sizeof__()+tasm.ProgramHeader.__sizeof__()


    wr             = tasm.Writer()
    wr.name        = "testexec"
    wr.header      = header


test()