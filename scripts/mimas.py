from PIL import Image
import struct


def convert_image(path:str)->None:
    im = Image.open(path)
    if (im.mode != "RGB" and im.mode != "RGBA"):
        print("Only RGB/RGBA images supported!")
        exit(1)
    width, height = im.size
    
    pixels = im.load()
    chunks:int = 0
    newfile_name = '.'.join(path.split(".")[:-1]+["mas"])
    file_handle = open(newfile_name, "wb")
    
    for line in range(0,height):
        for col in range(0, width):
            chunks+=1
            
    chunks:str = struct.pack("Q", chunks)
    
    file_handle.write(chunks)
    
    for line in range(0,height):
        for col in range(0, width):
            pixel = pixels[col, line]
            file_handle.write(struct.pack("BBB", pixel[0], pixel[1], pixel[2]))

    # chunk size = 3^2
    # chunks = width * height
    # total size = chunks * chunk_size

if __name__ == "__main__": convert_image("initramdir/img/saturn.png")