from os import walk as _walk, system as _sys, name as _osname
from configuration import *
import ramfs
import font


def src_obj(path:str=SRCPATH, ext:str=".c", exclude:list[str]=[SRCPATH+"limine"])->list[(str, str)]:
    result = []
    for (root,_,files) in _walk(path, topdown=True):
        if (root in exclude or ".git" in root):
            continue
        for file in files:
            if file.endswith(ext):
                result.append(
                    (root+"/"+file, BIN_DIR+file.replace(ext, ".o"))
                )
    return result

def clean(path:str=BIN_DIR)->None:
    _sys(f"rm -rf {path}*")

def limine_setup()->int:
    cmd = f"cp -v {KERNEL_OUTPUT} {LIMINE_CONF} {LIMINE_DIR}limine-bios.sys {LIMINE_DIR}limine-bios-cd.bin {LIMINE_DIR}limine-uefi-cd.bin {ISODIR}"
    print(f"Copying the kernel + limine...")
    exit_code = _sys(cmd)
    if (exit_code != 0): return exit_code
    
    print(f"Copying Limine EFI...")
    _sys(f"mkdir -p {ISODIR}EFI/BOOT")
    _sys(f"cp -v {LIMINE_DIR}BOOTX64.EFI {ISODIR}EFI/BOOT/")
    _sys(f"cp -v {LIMINE_DIR}BOOTIA32.EFI {ISODIR}EFI/BOOT/")
    
    cmd = f"xorriso {' '.join(XISOFLAGS)}"
    print(f"Creating ISO...")
    exit_code = _sys(cmd)
    if (exit_code != 0): return exit_code
    
    liminecmd = (LIMINE_DIR+"limine.exe" if _osname == "nt" else "limine")
    cmd = f" {liminecmd} bios-install {ISO_OUTPUT}"
    print(f"Installing Limine...")
    exit_code = _sys(cmd)
    if (exit_code != 0): return exit_code

    return 0

def fonts():
    header = font.bitmap_gen(FONT_PATH)
    open(f"{SRCPATH}graphics/include/fonts.h", "wb").write(header.encode())
    return 0

def setup()->None:
    _sys(f"mkdir -p {BIN_DIR}")
    _sys(f"mkdir -p {ISODIR}")

def build()->int:
    fonts()
    setup()
    
    sources = ()
    objects = ()
    sourcesasm = ()
    objectasm = ()
    
    try: sources, objects = zip(*src_obj())
    except ValueError: pass
    
    try: sourcesasm, objectasm = zip(*src_obj(SRCPATH, ".asm"))
    except ValueError: pass
    
    if sources and objects:
        clean()
        for source,object in zip(sources, objects):
            cmd = f"{CXX} {' '.join(CXXFLAGS)} {source} -o {object}"
            print(f"Building {source}...")
            exit_code = _sys(cmd)
            if (exit_code != 0): return exit_code
    
    if sourcesasm and objectasm:
        for source,object in zip(sourcesasm, objectasm):
            cmd = f"{AS} {' '.join(ASFLAGS)} {source} -o {object}"
            print(f"Building {source}...")
            exit_code = _sys(cmd)
            if (exit_code != 0): return exit_code

    if sources and objects:
        objects += objectasm
        cmd = f"{LD} {' '.join(LDFLAGS)} {' '.join(objects)} -o {KERNEL_OUTPUT}"
        print(f"Linking {(' '.join(objects))}...")
        exit_code = _sys(cmd)
        if (exit_code != 0): return exit_code
    
    ramfs.ramfs_generator()
    
    exit_code = limine_setup()
    if (exit_code != 0): return exit_code
    
    return 0


if __name__ == "__main__":
    exit(build())