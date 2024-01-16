
class Definition:
    MAGIC = b"\x3ATITAN"
    class Architecture: # ==> 4 bytes
        x86_64 = b"x\x86_\x64"
        i386   = b"i\x03\x08\x06"

        def __sizeof__() -> int:
            return len(Definition.Architecture.x86_64)

    class Version:      # ==> 1 byte
        current = b"\x01"

        def __sizeof__() -> int:
            return len(Definition.Version.current)

    class Order:        # ==> 1 byte (refers to instructions part)
        little  = b"\x01"
        big  = b"\x0A"

        def __sizeof__() -> int:
            return len(Definition.Order.little)

class Sizes:
    INTEGER32 = 4
    INTEGER8 = 1