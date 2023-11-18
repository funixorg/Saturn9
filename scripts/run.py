from configuration import ISO_OUTPUT, QEMU, QEMUFLAGS
from os import system as _sys

def qemu()->None:
    _sys(f"{QEMU} {' '.join(QEMUFLAGS)} -cdrom {ISO_OUTPUT}")

if __name__ == "__main__":
    qemu()