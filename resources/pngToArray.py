'''Program to convert png images into C-style array'''
''' run as follows: pngToArray.py [input_file] [output_file]'''

from PIL import Image
import argparse

def read_image(image_path):
    try:
        img = Image.open(image_path)
        return img
    except Exception as e:
        print("Error:", e)
        return None

def export_to_c_array(image, output_file):
    width, height = image.size
    pixels = list(image.getdata())
    
    with open(output_file, 'w') as f:
        f.write("#include <stdint.h>\n\n")
        #f.write("const uint32_t image_width = {};\n".format(width))
        #f.write("const uint32_t image_height = {};\n".format(height))
        f.write("const uint16_t {2}[{0}][{1}] =".format(height, width, output_file))
        
        f.write("   {\n")
        for y in range(height):
            f.write("    {")
            for x in range(width):
                pixel = pixels[y * width + x]
                r, g, b = pixel
                # Scale 8-bit channels to 5-bit (RGB) and 1-bit (A) for 16-bit color representation
                color = ((r >> 3) << 11) | ((g >> 2) << 5) | (b >> 3)
                f.write("0x{:04X}".format(color))
                if x != width - 1:
                    f.write(", ")
            f.write("}")
            if y != height - 1:
                f.write(",")
            f.write("\n")
        
        f.write("};\n")

def main():

    parser = argparse.ArgumentParser(
        prog= "pngToArray",
        description="Converts a png image into a C-Style array with 16 bit color",
        epilog="(c) 2024 Nicholas Biancolin - All Rights Reserved"
    )
    parser.add_argument('input_file', help='Path to image file')
    parser.add_argument('output_file', help='Path to output .h file')
    args = parser.parse_args()

    input_image = args.input_file
    output_file = args.output_file
    
    if output_file.endswith('/'):
        output_file += 'output'

    if not output_file.endswith('.h'):
        output_file += '.h'

    #input_image = "img/riskMap.png"
    #output_file = "image_data.h"
    
    image = read_image(input_image)
    if image:
        export_to_c_array(image, output_file)
        print("C-style array exported successfully to", output_file)

if __name__ == "__main__":
    main()
