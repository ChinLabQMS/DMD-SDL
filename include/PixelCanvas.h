#ifndef PIXELCANVAS_H
#define PIXELCANVAS_H

#include <iostream>
#include <cstring>
#include <cstdint>
#include <vector>
#include <numeric>
#include <omp.h>

// Convert subscripts to linear indices, use C-style indexing
inline int sub2ind(int nrows, int ncols, int x, int y) {
    return x * ncols + y;
}

// Convert projector space to real space
inline int realDiamondX(int nrows, int ncols, int x, int y) {
    return (nrows - x) / 2 + y;
}

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
    std::vector<int> Background2RealIndex; // Background to Real index mapping, size = BackgroundNumPixels
    std::vector<int> Real2PatternIndex; // Real to Pattern index mapping, size = RealNumPixels

    std::vector<uint32_t> PatternCanvas; // Pattern canvas, size = PatternNumPixels
    std::vector<uint32_t> RealCanvas; // Real canvas, size = RealNumPixels
    std::vector<std::vector<uint32_t>> PatternMemory; // Pattern memory, resizable

    ~PixelCanvas();
    void initCanvas(int nrows, int ncols, std::string arrangement = "Diamond", bool use_parallel = true);
    void resetBackground(uint32_t background_color = 0xFF0000, bool use_parallel = true);
    void resetPattern(uint32_t pattern_color = 0, bool use_parallel = true);
    void clearPatternMemory();
    void closeCanvas();
    void updatePattern2Real(bool use_parallel = true);
    void updateReal2Pattern(bool use_parallel = true);
    static std::vector<uint8_t> convertPattern2RGB(const uint32_t *pixels, size_t num_elements, bool use_parallel = true);
    static std::vector<uint32_t> convertRGB2Pattern(const uint8_t *rgb, size_t num_elements, bool alpha_mask = true, bool use_parallel = true);
};

#endif // PIXELCANVAS_H