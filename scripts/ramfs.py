import os

class File:
    def __init__(self, path:list[str]):
        self.path:list[str]=path
        self.base:int=0
        self.size:int=0
        self.content:str=""

class Directory:
    def __init__(self, path:list[str]):
        self.path:list[str]=path
        self.base:int=0
        self.size:int=0
        self.dirs:list[Directory]=[]
        self.files:list[File]=[]
        
def dir_parser(path, subdir, glob_base) -> (Directory, int):
    direc = Directory(subdir)
    dpath = '/'.join([path] + subdir)
    dbase = glob_base
    dsize = 0

    for item in os.listdir(dpath):
        rpath = os.path.join(dpath, item)

        if os.path.isfile(rpath):
            fpath = subdir + [item]
            source = open(rpath, "r").read()
            ssize = len(source)

            filec = File(fpath)
            filec.base = glob_base
            filec.size = ssize
            filec.content = source

            glob_base += ssize
            dsize += ssize

            direc.files.append(filec)
        elif os.path.isdir(rpath):
            dip, glob_base = dir_parser(path, subdir + [item], glob_base)
            dsize += dip.size
            direc.dirs.append(dip)

    direc.base = dbase
    direc.size = dsize

    return direc, glob_base

def print_dir(dirc: Directory, tab=0):
    print("\t" * tab + "-  " + str(dirc.path), str(dirc.base), str(dirc.size))
    for file in dirc.files:
        print("\t" * (tab + 1) + str(file.path), str(file.base), str(file.size))
    for dire in dirc.dirs:
        print_dir(dire, tab + 1)
        
        
def tree_parse(root: Directory, header:str, header_size:int, binary:str)->(str,int,str):
    header+=f"<{','.join(root.path)}> [{root.base};{root.size}] "
    header+="{"
    for file in root.files:
        header+=f"<{','.join(file.path)}>[{file.base};{file.size}]/"
    header=header[:-1]
    header+="}"
    for directory in root.dirs:
        header+="\n"
        header,header_size,binary = tree_parse(directory, header, len(header), binary)
    return header,len(header),binary

def generator(path: str = "initramdir") -> None:
    source_path: str = path
    header_size: int = 0
    glob_base: int = 0

    root, _ = dir_parser(path, [""], glob_base)

    print_dir(root)
    header,header_size,binary = tree_parse(root, "",0,"")
    i=0
    while i<header_size:
        print(header[i],end="")
        i+=1
    print("\n")
        

generator()