import os
import mimas
import struct

current_base = 0

fh_w = open("isodir/initramfs", "wb")
binary = b""

class File:
    def __init__(self, path:str, name:str):
        self.path:str=path
        self.base:int=0
        self.size:int=0
        self.name:str=name

class Directory:
    def __init__(self, path:str, name:str):
        self.path:str=path
        self.name:str=name
        self.base:int=0
        self.size:int=0
        self.directories:list[Directory]=[]
        self.files:list[File]=[]
        
def parse_dir(path:str,base_path:str) -> Directory:
    global current_base
    
    dpath=path.replace(base_path, "/").replace("//", "/")
    dname=dpath.split("/")[-1]
    if dname=="":
        dname="/"

    directory = Directory(dpath, dname)
    dir_base = current_base
    dir_size = 0

    dirs=[]
    
    for item in os.listdir(path):
        relative_path = os.path.join(path, item)
        
        if os.path.isfile(relative_path):
            ext=item.split(".")[-1]
            if (ext == "mas"): continue
            if (ext in ["png", "jpeg", "jpg"]):
                mimas.convert_image(relative_path)
                item='.'.join(item.split(".")[:-1]+["mas"])
            c_file = File(relative_path, item)
            c_file.base=current_base
            c_file.size = os.path.getsize(relative_path)
            current_base+= c_file.size
            directory.files.append(c_file)
            
            dir_size += c_file.size
        elif os.path.isdir(relative_path):
            dirs.append(relative_path)
    
    for subdir in dirs:
        subdirectory=parse_dir(subdir, base_path)
        dir_size+=subdirectory.size
        directory.directories.append(subdirectory)
    
    directory.base=dir_base
    directory.size=dir_size
    
    return directory

def parse_tree(directory: Directory, iteration: int = 0) -> None:
    global binary

    files_header = []
    for file in directory.files:
        file_struct = struct.pack(
            "64sQQ", file.name.encode("utf-8"), file.base, file.size
        )  # size = 80
        files_header.append(file_struct)
        data = open(file.path, "rb").read()
        binary+=data

    files_header = b"".join(files_header)

    dir_header = struct.pack(
        "32sQQl", directory.name.encode("utf-8"), directory.base, directory.size, len(directory.files)
    )

    header = dir_header + files_header
    return header


def ramfs_generator(path: str = "initramdir") -> None:
    global fh_w, binary, current_base
    binary=b""

    fh_w.seek(0, 0)
    fh_w.truncate()

    current_base = 0

    header = parse_tree(parse_dir(path, path))

    header_size = len(header)
    
    heading = struct.pack(f"i{header_size}s", header_size, header)

    fh_w.write(heading+binary)

    print("[RFS] Ramdisk generated!")

if __name__ == "__main__":
    ramfs_generator()