import os

current_base = 0
header = ""
binary = b""

class File:
    def __init__(self, path:str):
        self.path:str=path
        self.base:int=0
        self.size:int=0
        self.content:bytes=b""

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
            c_file = File(item)
            c_file.base=current_base
            c_file.content = open(relative_path, "rb").read()
            c_file.size = len(c_file.content)
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

def parse_tree(directories: list[Directory])->None:
    global header
    global binary
    for directory in directories:
        header+=f"<{directory.name}>[{directory.base};{directory.size}]|{len(directory.files)}|"
        file_header=[]
        for file in directory.files:
            file_header.append(f"<{file.path}>[{file.base};{file.size}]")
            binary+=file.content
        header+=f"({'&'.join(file_header)})"
        header+="{"
        for subdir in directory.directories:
            parse_tree([subdir])
            header+="="
        header=header[:-1]
        header+="}"

def ramfs_generator(path: str = "initramdir") -> None:
    global header
    global binary
    global current_base
    
    current_base = 0
    header = ""
    binary = b""
    
    parse_tree([parse_dir(path,path)])
    header_size = len(header)+1 # Size+Newline
    open("isodir/initramfs", "wb").write((str(header_size)+"\n"+header).encode('utf-8')+b"\n"+binary)
    print("[RFS] Ramdisk generated!")


ramfs_generator()