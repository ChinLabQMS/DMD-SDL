#include <PixelCanvas.h>

int main() {
    PixelCanvas canvas = PixelCanvas();
    canvas.initCanvas(2, 2, "Diamond");
    printf("Canvas created with width: %d, height: %d\n", canvas.NumCols, canvas.NumRows);
    for (int i = 0; i < canvas.PixelCount; ++i) {
        printf("PatternCanvas[%d] = %d\n", i, canvas.PatternCanvas[i]);
    }
    return 0;
}