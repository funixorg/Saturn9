from configuration import ISO_OUTPUT
from os import name as _osname

QEMU = ("qemu-system-x86_64.exe" if _osname == "nt" else "qemu-system-x86_64")
QEMUFLAGS:list[str] = [
    "-cdrom", f"{ISO_OUTPUT}",
    "-m 256", ('-L "C:\Program Files\qemu"' if _osname == "nt" else ""),
    "-no-reboot",
    "-no-shutdown",
    "-d", "int"
]

def qemu():
    