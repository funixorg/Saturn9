import os

current_base = 0
header = ""
binary = b""

class File:
    def __init__(self, path:list[str]):
        self.path:list[str]=path
        self.base:int=0
        self.size:int=0
        self.content:bytes=b""

class Directory:
    def __init__(self, path:list[str]):
        self.path:list[str]=path
        self.base:int=0
        self.size:int=0
        self.dirs:list[Directory]=[]
        self.files:list[File]=[]
        
def parse_dir(path:str,base_path:str) -> (Directory, int):
    global current_base
    
    directory = Directory(path.replace(base_path, "/"))
    dir_base = current_base
    dir_size = 0
    
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
    
    directory.base=dir_base
    directory.size=dir_size
    
    return directory

def parse_tree(directories: list[Directory]):
    global header
    global binary
    for directory in directories:
        header+=f"<{directory.path}>[{directory.base};{directory.size}]"
        file_header=[]
        dir_header=""
        for file in directory.files:
            file_header.append(f"<{file.path}>[{file.base};{file.size}]")
            binary+=file.content
        header+=f"({'&'.join(file_header)})"

def generator(path: str = "initramdir") -> None:
    parse_tree([parse_dir(path,path)])
    header_size = len(header)+1 # Size+Newline
    open("isodir/initramfs", "wb").write((str(header_size)+"\n"+header).encode('utf-8')+b"\n"+binary)
    print("[RFS] initramfs generated!")

generator()