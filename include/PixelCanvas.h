#ifndef PIXELCANVAS_H
#define PIXELCANVAS_H

#include <iostream>
#include <cstring>
#include <cstdint>
#include <cmath>
#include <vector>
#include <numeric>
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
protected:
    int NumRows = 0; // Number of rows in the pattern canvas
    int NumCols = 0; // Number of columns in the pattern canvas
    int RealNumRows = 0; // Number of rows in the real canvas
    int RealNumCols = 0; // Number of columns in the real canvas
    int PatternNumPixels = 0; // Number of pixels in the pattern canvas
    int BackgroundNumPixels = 0; // Number of pixels in the background canvas (real - pattern)
    int RealNumPixels = 0; // Number of pixels in the real canvas
    std::string PixelArrangement = "Diamond"; // Pixel arrangement, default to Diamond
    std::vector<int> Pattern2RealIndex; // Pattern to Real index mapping, size = PatternNumPixels
    std::vector<int> Background2RealIndex; // Background to Real index mapping, size = BackgroundNumPixels
    std::vector<int> Real2PatternIndex; // Real to Pattern index mapping, size = RealNumPixels
public:
    std::vector<uint32_t> PatternCanvas; // Pattern canvas, size = PatternNumPixels
    std::vector<std::vector<uint32_t>> PatternMemory; // Pattern memory, resizable    
    void initCanvas(int nrows, int ncols, std::string arrangement, bool use_parallel);
    void closeCanvas();
    void clearPatternMemory();
    void clearPatternMemory(size_t index);
    void drawPixelsOnReal(std::vector<int> real_idx, uint32_t color, bool use_parallel);
    void drawPixelsOnRealBit(std::vector<int> real_idx, int bit_plane, bool color, bool use_parallel);
    void drawLineOnReal(double A, double B, double C, double d, uint32_t color, bool use_parallel);
    void drawCircleOnReal(double x0, double y0, double r, uint32_t color, bool use_parallel);
    std::vector<uint8_t> getPatternCanvasRGB(bool use_parallel);
    std::vector<uint8_t> getRealCanvasRGB(bool use_parallel);
    std::vector<uint8_t> getPatternMemoryRGB(size_t index, bool use_parallel);
    std::vector<uint32_t> convertPattern2Real(const uint32_t *pattern, uint32_t background_color, bool use_parallel);
    static std::vector<uint8_t> convertPattern2RGB(uint8_t *pattern, int height, int width, int pitch, bool use_parallel);
    static std::vector<uint32_t> convertRGB2Pattern(uint8_t *rgb, int height, int width, int pitch, bool use_parallel);
    static std::vector<uint32_t> createSolidPattern(uint32_t color, int num_elements, bool use_parallel);
};

#endif // PIXELCANVAS_H