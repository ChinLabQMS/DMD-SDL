from PIL import Image

def square4(img,col,row):
    pixels = img.load()
    for i in range(2):
        for j in range(2):
            pixels[col+i,row+j]=0
    return img

def star5(img,col,row):
    pixels = img.load()
    for i in range(-1,2):
        pixels[col,row+i]=0
        pixels[col+i,row]=0
    return img
    
def square9(img,col,row):
    pixels = img.load()
    for i in range(-1,2):
        for j in range(-1,2):
            pixels[col+i,row+j]=0
    return img
    
def star12(img,col,row):
    pixels = img.load()
    for i in range(-1,3):
        pixels[col,row+i]=0
        pixels[col+1,row+i]=0
        pixels[col+i,row]=0
        pixels[col+i,row+1]=0
    return img
    
def star13(img,col,row):
    img = square9(img,col,row)
    pixels = img.load()

    pixels[col+2,row]=0
    pixels[col-2,row]=0
    pixels[col,row+2]=0
    pixels[col,row-2]=0
    
    return img

def square16(img,col,row):
    pixels = img.load()

    for i in range(4):
        for j in range(4):
            pixels[col+i,row+j]=0
            
    return img

def star21(img,col,row):
    pixels = img.load()

    for i in range(3):
        pixels[col-1,row+1+i]=0
        pixels[col+3,row+1+i]=0
        for j in range(5):
            pixels[col+i,row+j]=0
               
    return img

def star24(img,col,row):
    img = square16(img,col,row)
    pixels = img.load()

    for i in range(2):
        pixels[col+1+i,row-1]=0
        pixels[col+1+i,row+4]=0
        pixels[col-1,row+1+i]=0
        pixels[col+4,row+1+i]=0
               
    return img

def star29(img,col,row):
    pixels = img.load()

    for i in range(5):
        for j in range(5):
            pixels[col+i,row+j]=0

    pixels[col-1,row+2]=0
    pixels[col+2,row-1]=0
    pixels[col+2,row+5]=0
    pixels[col+5,row+2]=0  
                    
    return img
    
def star32(img,col,row):
    pixels = img.load()

    for i in range(4):
        for j in range(6):
            pixels[col+i,row+j]=0
            
    for n in range(4):
        pixels[col-1,row+1+n]=0
        pixels[col+4,row+1+n]=0
                    
    return img

def star37(img,col,row):
    pixels = img.load()

    for i in range(5):
        for j in range(5):
            pixels[col+i,row+j]=0
            
    for n in range(3):
        pixels[col-1,row+1+n]=0
        pixels[col+5,row+1+n]=0
        pixels[col+1+n,row-1]=0
        pixels[col+1+n,row+5]=0
                                        
    return img
    
def star44(img,col,row):
    pixels = img.load()

    for i in range(6):
        for j in range(6):
            pixels[col+i,row+j]=0
            
    for n in range(2):
        pixels[col-1,row+2+n]=0
        pixels[col+6,row+2+n]=0
        pixels[col+2+n,row-1]=0
        pixels[col+2+n,row+6]=0
                                        
    return img
    
def star49(img,col,row):
    pixels = img.load()

    for i in range(7):
        for j in range(7):
            pixels[col+i,row+j]=0
    
    color = pixels[col-1,row]
    pixels[col,row]=color
    pixels[col,row+6]=color
    pixels[col+6,row]=color
    pixels[col+6,row+6]=color
    
    pixels[col+3,row-1]=0
    pixels[col-1,row+3]=0
    pixels[col+7,row+3]=0
    pixels[col+3,row+7]=0
                                            
    return img
    
def star52(img,col,row):
    pixels = img.load()

    for i in range(6):
        for j in range(6):
            pixels[col+i,row+j]=0
            
    for n in range(4):
        pixels[col-1,row+1+n]=0
        pixels[col+6,row+1+n]=0
        pixels[col+1+n,row-1]=0
        pixels[col+1+n,row+6]=0
                    
    return img
 
def star68(img,col,row):
    pixels = img.load()

    for i in range(8):
        for j in range(8):
            pixels[col+i,row+j]=0
    
    color = pixels[col-1,row]
    pixels[col,row]=color
    pixels[col,row+7]=color
    pixels[col+7,row]=color
    pixels[col+7,row+7]=color
    
    for n in range(2):
        pixels[col+3+n,row-1]=0
        pixels[col-1,row+3+n]=0
        pixels[col+8,row+3+n]=0
        pixels[col+3+n,row+8]=0
                                            
    return img   
    
def star69(img,col,row):
    pixels = img.load()

    for i in range(7):
        for j in range(7):
            pixels[col+i,row+j]=0
    
    for n in range(5):
        pixels[col-1,row+1+n]=0
        pixels[col+7,row+1+n]=0
        pixels[col+1+n,row-1]=0
        pixels[col+1+n,row+7]=0
                                            
    return img 
    
def star81(img,col,row):
    pixels = img.load()

    for i in range(7):
        for j in range(7):
            pixels[col+i,row+j]=0
    
    for n in range(7):
        pixels[col-1,row+n]=0
        pixels[col+7,row+n]=0
        pixels[col+n,row-1]=0
        pixels[col+n,row+7]=0
        
    pixels[col-2,row+3]=0
    pixels[col+8,row+3]=0
    pixels[col+3,row-2]=0
    pixels[col+3,row+8]=0
                                            
    return img    

def star88(img,col,row):
    pixels = img.load()

    for i in range(8):
        for j in range(8):
            pixels[col+i,row+j]=0
            
    for n in range(6):
        pixels[col-1,row+1+n]=0
        pixels[col+8,row+1+n]=0
        pixels[col+1+n,row-1]=0
        pixels[col+1+n,row+8]=0
                            
    return img
    
def star97(img,col,row):
    pixels = img.load()

    for i in range(9):
        for j in range(9):
            pixels[col+i,row+j]=0
            
    for n in range(5):
        pixels[col-1,row+2+n]=0
        pixels[col+9,row+2+n]=0
        pixels[col+2+n,row-1]=0
        pixels[col+2+n,row+9]=0
        
    color = pixels[col-1,row]  
    pixels[col,row]=color
    pixels[col+8,row]=color
    pixels[col,row+8]=color
    pixels[col+8,row+8]=color
                                                
    return img

def star104(img,col,row):
    pixels = img.load()

    for i in range(10):
        for j in range(10):
            pixels[col+i,row+j]=0
            
    for n in range(2):
        pixels[col-1,row+4+n]=0
        pixels[col+10,row+4+n]=0
        pixels[col+4+n,row-1]=0
        pixels[col+4+n,row+10]=0
        
    color = pixels[col-1,row]  
    pixels[col,row]=color
    pixels[col+9,row]=color
    pixels[col,row+9]=color
    pixels[col+9,row+9]=color
                                                
    return img
    
def star113(img,col,row):
    pixels = img.load()

    for i in range(9):
        for j in range(9):
            pixels[col+i,row+j]=0
            
    for n in range(7):
        pixels[col-1,row+1+n]=0
        pixels[col+9,row+1+n]=0
        pixels[col+1+n,row-1]=0
        pixels[col+1+n,row+9]=0
     
    pixels[col+4,row-2]=0  
    pixels[col+4,row+10]=0
    pixels[col-2,row+4]=0
    pixels[col+10,row+4]=0  
                               
    return img
        
        
def star140(img,col,row):
    pixels = img.load()

    for i in range(10):
        for j in range(10):
            pixels[col+i,row+j]=0
            
    for n in range(8):
        pixels[col-1,row+1+n]=0
        pixels[col+10,row+1+n]=0
        pixels[col+1+n,row-1]=0
        pixels[col+1+n,row+10]=0
    
    for n in range(2):
        pixels[col-2,row+4+n]=0
        pixels[col+11,row+4+n]=0
        pixels[col+4+n,row-2]=0
        pixels[col+4+n,row+11]=0
        
def star164(img,col,row):
    pixels = img.load()

    for i in range(12):
        for j in range(12):
            pixels[col+i,row+j]=0
            
    for n in range(6):
        pixels[col-1,row+3+n]=0
        pixels[col+12,row+3+n]=0
        pixels[col+3+n,row-1]=0
        pixels[col+3+n,row+12]=0
    
    color = pixels[col-1,row]
    pixels[col,row]=color
    pixels[col+11,row]=color
    pixels[col,row+11]=color
    pixels[col+11,row+11]=color
    
    return img