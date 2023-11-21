from PIL import Image, ImageDraw, ImageFont
import sys, os
from configuration import FONT_WIDTH, FONT_HEIGHT

font_size = 16
font_width = FONT_WIDTH
font_height = FONT_HEIGHT

def convert_to_bitmap_array(bitmap_image):
    width, height = bitmap_image.size
    pixel_data = list(bitmap_image.getdata())

    bitmap_array = []
    for y in range(height):
        row = 0
        for x in range(width):
            pixel = pixel_data[y * width + x]
            row = row << 1
            if pixel == 0:  # Assuming black pixels represent the letter
                row = row | 1
        bitmap_array.append(row ^ 0xFF)  # Inverting 1s and 0s using XOR with 0xFF

    return bitmap_array

def generate_c_array(bitmap_array):
    c_array = "{\n"
    for row in bitmap_array:
        c_array += f"    0b{row:08b},\n"
    c_array += "},\n"
    return c_array

def generate_c_array_from_ttf(font_path): 
    characters_per_row = 32  # Assuming 32 characters per row

    img = Image.new('1', (8 * characters_per_row, font_size * 16))
    draw = ImageDraw.Draw(img)
    font = ImageFont.truetype(font_path, font_size)

    all_characters_bitmap = []
    for char in range(256):  # ASCII range from 0 to 255
        character = chr(char)
        draw.text(((char % characters_per_row) * 8, (char // characters_per_row) * font_size), character, font=font, fill=1)
        character_img = img.crop(((char % characters_per_row) * 8, (char // characters_per_row) * font_size, ((char % characters_per_row) + 1) * 8, ((char // characters_per_row) + 1) * font_size))
        character_bitmap = convert_to_bitmap_array(character_img)
        all_characters_bitmap.append(character_bitmap)

    return all_characters_bitmap

def bitmap_gen(font_path:str)->str:
    if not os.path.exists(font_path):
        print(f"Font file not found at: {font_path}")
        exit(1)
    ttf_font_path = font_path
    all_characters = generate_c_array_from_ttf(ttf_font_path)
    
    output = f"#include <stdint.h>\n#ifndef FONT_H\n#define FONT_H\nconst int font_width = {font_width};\nconst int font_height = {font_height};\n\n"

    output += f"uint8_t font[{len(all_characters)}][{font_size}] = {{\n"
    for bitmap_array in all_characters:
        c_array = generate_c_array(bitmap_array)
        output+=c_array
    output += "};\n\n#endif\n"
    return output
