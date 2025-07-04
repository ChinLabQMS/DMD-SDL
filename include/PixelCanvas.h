#ifndef PIXELCANVAS_H
#define PIXELCANVAS_H

#include <iostream>
#include <cstring>
#include <cstdint>
#include <cmath>
#include <vector>
#include <numeric>
#include <algorithm>
#include <omp.h>

// Convert subscripts to linear indices, use C-style indexing
inline int sub2ind(int nrows, int ncols, int x, int y) {
    return x * ncols + y;
}

// Convert projector space to real space, row index
inline int realDiamondX(int nrows, int ncols, int x, int y) {
    return (nrows - x) / 2 + y;
}

// Convert projector space to real space, column index
inline int realDiamondY(int nrows, int ncols, int x, int y) {
    return (nrows - 1 - x) / 2 + ncols - y - 1;
}

class PixelCanvas {
public:
    int NumRows = 0; // Number of rows in the pattern canvas
    int NumCols = 0; // Number of columns in the pattern canvas
    int RealNumRows = 0; // Number of rows in the real canvas
    int RealNumCols = 0; // Number of columns in the real canvas
    int PatternNumPixels = 0; // Number of pixels in the pattern canvas
    int BackgroundNumPixels = 0; // Number of pixels in the background canvas (real - pattern)
    int RealNumPixels = 0; // Number of pixels in the real canvas
    std::string PixelArrangement = "Diamond"; // Pixel arrangement, default to Diamond
    std::vector<int> Pattern2RealIndex; // Pattern to Real index mapping, size = PatternNumPixels
    std::vector<int> Real2PatternIndex; // Real to Pattern index mapping, size = RealNumPixels
    std::vector<std::vector<uint32_t>> PatternMemory; // Pattern memory, resizable, to store patterns loaded from files
    std::vector<std::vector<uint32_t>> DynamicMemory; // Dynamic memory, resizable, to store patterns computed on the fly
    void initCanvas(int nrows, int ncols, std::string arrangement, bool use_parallel);
    void closeCanvas();
    void resetDynamicMemory(uint32_t color, bool use_parallel);
    void clearDynamicMemoryAll();
    void clearPatternMemoryAll();
    void drawPixelsDynamic(int index, std::vector<int> pattern_idx, uint32_t color, bool use_parallel);
    void drawPixelsDynamicBit(int index, int bit_plane, std::vector<int> pattern_idx, bool color, bool use_parallel);
    void generateBlackTweezerPattern(int num_tweezers, const double *x0, const double *y0, double r, double dx, double dy, int num_RGB_buffers, int num_frames, bool use_parallel);
    void generateBlackTweezerPatternStatic(std::vector<double> calib, 
                                           std::vector<double> x0, 
                                           std::vector<double> y0,
                                           double radius);
    std::vector<int> drawCirclesOnReal(int num_circles, const double* x0, const double* y0, double r, bool use_parallel);
    std::vector<uint8_t> getDynamicMemoryRGB(int index, bool use_parallel);
    std::vector<uint32_t> getDynamicMemoryReal(int index, uint32_t background_color, bool use_parallel);
    std::vector<uint8_t> getDynamicMemoryRealRGB(int index, uint32_t background_color, bool use_parallel);
    std::vector<uint8_t> getPatternMemoryRGB(int index, bool use_parallel);
    std::vector<uint32_t> getPatternMemoryReal(int index, uint32_t background_color, bool use_parallel);
    std::vector<uint8_t> getPatternMemoryRealRGB(int index, uint32_t background_color, bool use_parallel);
    std::vector<uint32_t> convertPattern2Real(const uint32_t *pattern, uint32_t background_color, bool use_parallel);
    static std::vector<uint8_t> convertPattern2RGB(const uint32_t *pattern, int height, int width, int pitch, bool use_parallel);
    static std::vector<uint32_t> convertRGB2Pattern(const uint8_t *rgb, int height, int width, int pitch, bool use_parallel);
};

#endif // PIXELCANVAS_H
