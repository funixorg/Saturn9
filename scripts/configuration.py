SRCPATH="saturn9/"
LINKSCRIPT=SRCPATH+"cfg/linker.ld"
LIMINE_CONF = SRCPATH+"cfg/limine.cfg"
LIMINE_DIR = SRCPATH+"limine/"
INCLUDE_DIRS = [LIMINE_DIR, SRCPATH+"fs/include/", SRCPATH+"mem/include/"]

CXX = "x86_64-linux-gnu-gcc"
CXXFLAGS = [
    "-Wall",
    "-Wextra",
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

LD = "x86_64-linux-gnu-ld"
LDFLAGS = [
    "-m elf_x86_64",
    "-nostdlib",
    "-static",
    "-pie",
    "--no-dynamic-linker",
    "-z text",
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