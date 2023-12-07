from os import name as _osname

SRCPATH="saturn9/"
LINKSCRIPT=SRCPATH+"cfg/linker.ld"
LIMINE_CONF = SRCPATH+"cfg/limine.cfg"
LIMINE_DIR = SRCPATH+"limine/"
FONT_WIDTH = 9
FONT_HEIGHT = 16
FONT_PATH = "scripts/fonts/phoenix9x16.ttf"

INCLUDE_DIRS = [
    LIMINE_DIR,
    SRCPATH+"fs/include/",
    SRCPATH+"mem/include/",
    SRCPATH+"graphics/include/",
    SRCPATH+"common/include/",
    SRCPATH+"interrupts/include/",
    SRCPATH+"ports/include/",
    SRCPATH+"layouts/",
    SRCPATH+"user/include/",
    SRCPATH+"pit/include/",
    SRCPATH+"rhea/",
]

CXX = "x86_64-linux-gnu-gcc"
CXXFLAGS = [
    "-Wall",
    "-Wno-char-subscripts",
    "-Wno-pointer-to-int-cast",
    "-std=gnu11",
    "-ffreestanding",
    "-fno-stack-protector",
    "-fno-stack-check",
    "-fno-lto",
    "-fPIE",
    "-m64",
    "-march=x86-64",
    "-mno-80387",
    "-mno-mmx",
    "-mno-sse",
    "-mno-sse2",
    "-mno-red-zone",
    "-c",
    ' '.join([f'-I./{idir}' for idir in INCLUDE_DIRS])
]

AS = "nasm"
ASFLAGS = [
    "-f elf64"
]

LD = "ld"
LDFLAGS = [
    "-m elf_x86_64",
    "-nostdlib",
    "-static",
    "--no-dynamic-linker",
    "-z notext",
    "-z max-page-size=0x1000",
    f"-T {LINKSCRIPT}"
]

BIN_DIR = "bin/"
KERNEL_OUTPUT = BIN_DIR+"saturn.bin"
ISODIR = "isodir/"
ISO_OUTPUT = BIN_DIR+"saturn.iso"

XISOFLAGS = [
    "-as mkisofs",
    "-b limine-bios-cd.bin",
    "-no-emul-boot",
    "-boot-load-size 4",
    "-boot-info-table",
    "--efi-boot limine-uefi-cd.bin",
    "--efi-boot-part",
    "--efi-boot-image",
    "--protective-msdos-label",
    f"{ISODIR}",
    f"-o {ISO_OUTPUT}"
]

QEMU = ("qemu-system-x86_64.exe" if _osname == "nt" else "qemu-system-x86_64")
QEMUFLAGS:list[str] = [
    "-m 128",
    ('-L "C:\Program Files\qemu"' if _osname == "nt" else ""),
    "-no-reboot",
    "-no-shutdown",
    "-serial stdio",
    #"-d int",
    #"-D qemu_log.txt",
    ("-L scripts/OVMF.fd" if _osname != "nt" else ""),
    "-smp 4",
    #"-s -S"
]

