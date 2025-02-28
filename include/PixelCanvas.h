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
    int NumRows = 0;
    int NumCols = 0;
    std::string PixelArrangement = "Diamond";
    int RealNumRows = 0;
    int RealNumCols = 0;
    int PixelCount = 0;
    int BackgroundCount = 0;
    int RealPixelCount = 0;

    std::vector<int> PixelIndex;
    std::vector<int> RealPixelIndex;
    std::vector<int> RealBackgroundIndex;

    std::vector<uint32_t> PatternCanvas;
    std::vector<uint32_t> RealPatternCanvas;

    ~PixelCanvas();
    void initCanvas(int nrows, int ncols, std::string arrangement = "Diamond", bool use_parallel = true);
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