# Modified from Misha's code on an old DMD model

#Python Imaging Library
#https://pillow.readthedocs.io/en/5.1.x/
#Provides an easy way to work with different image formats
from PIL import Image

#Algorithms for computing rows and columns in real space that correspond to
#row DMDRow and column DMDCol in DMD space
def rowAlgorithm(DMDCol,DMDRow):
    return -DMDCol + int(DMDRow/2)
    
def columnAlgorithm(DMDCol,DMDRow):
    return  int((DMDRow+1)/2)+DMDCol  

#This will convert the template image into a DMD image
def convertImageToDMDArray(image):
    pixels = image.load()
 
    numCols = 608
    numRows = 684    
          
    #Create a DMDImg image of the correct dimensions
    DMDImg = Image.new("1",(numCols,numRows),color=1)
    DMDPixels = DMDImg.load()    

    #Loop through every column and row for the DMD image and assign it 
    #the corresponding pixel value from the real space image
    for column in range(numCols):
        for row in range(numRows):
                        
            columnRealSpace = columnAlgorithm(column,row) 
            RowRealSpace = 607 + rowAlgorithm(column,row)

            DMDPixels[column,row]=pixels[columnRealSpace,RowRealSpace]
            
    return DMDImg 

if __name__ == '__main__':
    #Specify the path where the template is and load the template
    path = "C:/Users/Admin/Desktop/DifferentProjects/DMDContrast/"
    image = Image.open(path+'DMDLayoutTemplate.bmp')

    #Convert the template to a DMD Image
    dmdImg = convertImageToDMDArray(image)

    #Show the converted DMD pattern and save it to your directory
    dmdImg.show()
    dmdImg.save(path+"DMDExample1.bmp")