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
    bool IsCanvasInitialized = false;
    int NumRows, NumCols;
    std::string PixelArrangement;
    int RealNumRows, RealNumCols;
    int PixelCount, BackgroundCount, RealPixelCount;

    std::vector<int> PixelIndex;
    std::vector<int> RealPixelIndex;
    std::vector<int> RealBackgroundIndex;

    std::vector<uint32_t> PatternCanvas;
    std::vector<uint32_t> RealPatternCanvas;

    ~PixelCanvas();
    void initCanvas(int nrows, int ncols, std::string arrangement = "Diamond");
    void resetCanvas(uint32_t pattern_color = 0, uint32_t background_color = 0xFF0000, bool use_parallel = true);
    void closeCanvas();
    void updatePattern2Real(bool use_parallel = true);
    void updateReal2Pattern(bool use_parallel = true);
    void copyPixel2Pattern(uint32_t* pixels, size_t num_pixels, bool use_parallel = true);
    void copyPixel2Real(uint32_t* pixels, size_t num_pixels, bool use_parallel = true);
    static std::vector<uint8_t> convertPattern2RGB(const uint32_t *pixels, size_t num_elements, bool use_parallel = true);
    static std::vector<uint32_t> convertRGB2Pattern(const uint8_t *rgb, size_t num_elements, bool alpha_mask = true, bool use_parallel = true);
};

#endif // PIXELCANVAS_H