from PIL import Image, ImageDraw, ImageFont

from tkinter import Tk
from tkinter.filedialog import askopenfilename

import math
import os

DMD_ROWS = 1140
DMD_COLS = 912

REAL_ROWS = math.ceil((DMD_ROWS-1) / 2) + DMD_COLS
REAL_COLS = DMD_COLS + (DMD_ROWS-1) // 2

# Convert DMD space (row, col) to real space
def realSpaceCoor(row, col):
    return (math.ceil(row / 2) + col, DMD_COLS - 1 + row//2 - col)

def generateTemplate():
    template = Image.new("RGB", size=(REAL_COLS, REAL_ROWS), color='#ff0000')

    for row in range(DMD_ROWS):
        for col in range(DMD_COLS):
            real_row, real_col = realSpaceCoor(row, col)
            template.putpixel((real_col, real_row), value=(255, 255, 255))

    draw = ImageDraw.Draw(template)
    font = ImageFont.truetype("arial.ttf", 30)

    # Add labels on the corners
    corner00 = realSpaceCoor(0, 0)[1] - 100, realSpaceCoor(0, 0)[0]
    corner10 = realSpaceCoor(DMD_ROWS-1, 0)[1] - 150, realSpaceCoor(DMD_ROWS-1, 0)[0] + 150
    corner11 = realSpaceCoor(DMD_ROWS-1, DMD_COLS-1)[1] + 50, realSpaceCoor(DMD_ROWS-1, DMD_COLS-1)[0] - 50

    draw.text(corner00, '(0, 0)', font=font, fill=0)
    draw.text(corner10, f'({DMD_ROWS-1}, 0)', font=font, fill=0)
    draw.text(corner11, f'({DMD_ROWS-1}, {DMD_COLS-1})', font=font, fill=0)

    template.save(f'DMD_template_{DMD_ROWS}x{DMD_COLS}.bmp')

def generatePattern(row=DMD_ROWS//2, col=DMD_COLS//2):
    pass

def convertImageToDMDArray(image):
    pixels = image.load()

    #Create a DMD image of the correct dimensions
    dmd_image = Image.new("1", (DMD_COLS, DMD_ROWS), color=1)  

    #Loop through every column and row for the DMD image and assign it 
    #the corresponding pixel value from the real space image
    for row in range(DMD_ROWS):
        for col in range(DMD_COLS):
            real_row, real_col = realSpaceCoor(row, col)
            dmd_image.putpixel((col, row), value=pixels[real_col, real_row])
    return dmd_image

if __name__ == '__main__':
    # generateTemplate()

    root = Tk()
    root.withdraw()
    file_path = askopenfilename(title='Select Modified BMP Image Template', filetypes=[('BMP Files', '*.bmp')])
    directory, filename = os.path.split(file_path)
    filename_new = 'DMD_pattern_' + os.path.splitext(filename)[0] + '.bmp'

    image = Image.open(file_path)
    image = image.convert('1')

    #Convert the template to a DMD Image
    dmd_image = convertImageToDMDArray(image)

    #Show the converted DMD pattern and save it to your directory
    dmd_image.show()
    dmd_image.save(os.path.join(directory, filename_new))