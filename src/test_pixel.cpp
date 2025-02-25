#include <PixelCanvas.h>

int main() {
    PixelCanvas canvas = PixelCanvas();
    canvas.initCanvas(2, 2);
    printf("Canvas created with width: %d, height: %d, real_width: %d, real_height: %d\n", 
        canvas.NumCols, canvas.NumRows, canvas.RealNumCols, canvas.RealNumRows);
    for (int i = 0; i < canvas.PixelCount; ++i) {
        printf("PatternCanvas[%d] = %d\n", i, canvas.PatternCanvas[i]);
    }
    canvas.resetCanvas(0b111111110000000000000000, 0);
    for (int i = 0; i < canvas.PixelCount; ++i) {
        printf("PatternCanvas[%d] = %d\n", i, canvas.PatternCanvas[i]);
    }
    for (int i = 0; i < canvas.RealPixelCount; ++i) {
        printf("RealPatternCanvas[%d] = %d\n", i, canvas.RealPatternCanvas[i]);
    }
    canvas.closeCanvas();
    return 0;
}