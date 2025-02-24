#ifndef PIXELCANVAS_H
#define PIXELCANVAS_H

#include <iostream>
#include <cmath>
#include <cstdint>
#include <omp.h>

class PixelCanvas {
public:
    int NumRows, NumCols;
    std::string PixelArrangement;
    int RealNumRows, RealNumCols;
    int PixelCount, BackgroundCount, RealPixelCount;

    // Indices for real projector space
    int *PixelIndex = NULL;
    int *RealPixelIndex = NULL;
    int *RealBackgroundIndex = NULL;

    // Canvas for pattern creation
    uint32_t *PatternCanvas = NULL;
    uint32_t *RealPatternCanvas = NULL;

    ~PixelCanvas();
    void initCanvas(int nrows, int ncols, const std::string arrangement = "Diamond");
    void resetCanvas(uint32_t pattern_color = 0, uint32_t background_color = 0b111111110000000000000000);
    void closeCanvas();
    void updatePattern2Real();
    void updateReal2Pattern();
    void copyPixel2Pattern(uint32_t *pixels);
    void copyPixel2Real(uint32_t *pixels);
};

#endif // PIXELCANVAS_H