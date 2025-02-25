#ifndef PIXELCANVAS_H
#define PIXELCANVAS_H

#include <iostream>
#include <cstring>
#include <cstdint>
#include <vector>
#include <numeric>
#include <omp.h>

class PixelCanvas {
public:
    int NumRows, NumCols;
    std::string PixelArrangement;
    int RealNumRows, RealNumCols;
    int PixelCount, BackgroundCount, RealPixelCount;

    std::vector<int> PixelIndex;
    std::vector<int> RealPixelIndex;
    std::vector<int> RealBackgroundIndex;

    std::vector<uint32_t> PatternCanvas;
    std::vector<uint32_t> RealPatternCanvas;

    explicit PixelCanvas(std::string arrangement = "Diamond");
    ~PixelCanvas();
    void initCanvas(int nrows, int ncols);
    void resetCanvas(uint32_t pattern_color = 0, uint32_t background_color = 0xFF0000);
    void closeCanvas();
    void updatePattern2Real(bool fast = true);
    void updateReal2Pattern(bool fast = true);
    void copyPixel2Pattern(uint32_t* pixels, size_t numPixels);
    void copyPixel2Real(uint32_t* pixels, size_t num_pixels);
};

#endif // PIXELCANVAS_H