from PIL import Image
import struct


def convert_image(path:str)->None:
    im = Image.open(path)
    if (im.mode != "RGB" and im.mode != "RGBA"):
        print("Only RGB/RGBA images supported!")
        exit(1)
    width, height = im.size
    
    pixels = im.load()
    newfile_name = '.'.join(path.split(".")[:-1]+["mas"])
    file_handle = open(newfile_name, "wb")
            
    size:str = struct.pack("QQ", width,height)
    
    file_handle.write(size)
    
    for line in range(0,height):
        for col in range(0, width):
            pixel = pixels[col, line]
            file_handle.write(struct.pack("iii", pixel[0], pixel[1], pixel[2]))
            
            
    file_handle.close()
    
    print(f"[MIMAS] {newfile_name} ({width}x{height})")

    # chunk size = 3^2
    # chunks = width * height
    # total size = chunks * chunk_size

if __name__ == "__main__": convert_image("initramdir/img/saturn.png")