#include <PixelCanvas.h>

// Initialize Canvas to a specific arrangement and size
// Compute forward and reverse mapping between pattern and real canvas
void PixelCanvas::initCanvas(int nrows, int ncols, std::string arrangement, bool use_parallel) {
    PixelArrangement = std::move(arrangement);
    NumRows = nrows;
    NumCols = ncols;
    PatternNumPixels = NumRows * NumCols;
    Pattern2RealIndex.resize(PatternNumPixels);
    // Assign pattern to real index mapping based on arrangement
    if (PixelArrangement == "Square") {
        RealNumRows = NumRows;
        RealNumCols = NumCols;
        RealNumPixels = PatternNumPixels;
        #pragma omp parallel for if(use_parallel)
        for (int i = 0; i < PatternNumPixels; ++i) {
            Pattern2RealIndex[i] = i;
        }
    }
    else if (PixelArrangement == "Diamond") {
        RealNumRows = std::max(0, nrows / 2 + ncols);
        RealNumCols = std::max(0, ncols + (nrows - 1) / 2);
        RealNumPixels = RealNumRows * RealNumCols;
        #pragma omp parallel for if(use_parallel)
        for (int i = 0; i < PatternNumPixels; ++i) {
            int x = i / ncols;  // row index
            int y = i % ncols;  // column index
            Pattern2RealIndex[i] = sub2ind(RealNumRows, RealNumCols, 
                                           realDiamondX(nrows, ncols, x, y), 
                                           realDiamondY(nrows, ncols, x, y));
        }
    }
    // Assign real to pattern index mapping
    Real2PatternIndex.resize(RealNumPixels);
    #pragma omp parallel for if(use_parallel)
    for (int i = 0; i < RealNumPixels; ++i) {
        Real2PatternIndex[i] = -1;
    }
    #pragma omp parallel for if(use_parallel)
    for (int i = 0; i < PatternNumPixels; ++i) {
        Real2PatternIndex[Pattern2RealIndex[i]] = i;
    }
    BackgroundNumPixels = RealNumPixels - PatternNumPixels;
    clearPatternMemoryAll();
    clearDynamicMemoryAll();
    resetDynamicMemory(0xFFFFFFFF, use_parallel);
}

// Cleanup Function
void PixelCanvas::closeCanvas() {
    PixelArrangement.clear();
    Pattern2RealIndex.clear();
    Real2PatternIndex.clear();
    clearPatternMemoryAll();
    clearDynamicMemoryAll();
    NumRows = 0;
    NumCols = 0;
    RealNumRows = 0;
    RealNumCols = 0;
    PatternNumPixels = 0;
    BackgroundNumPixels = 0;
    RealNumPixels = 0;
}

// Reset last canvas to a specific color
void PixelCanvas::resetDynamicMemory(uint32_t color, bool use_parallel) {
    if (DynamicMemory.empty()) {
        DynamicMemory.resize(1);
        DynamicMemory[0].resize(PatternNumPixels, color);
        return;
    }
    int last_index = DynamicMemory.size() - 1;
    #pragma omp parallel for if(use_parallel)
    for (int i = 0; i < PatternNumPixels; ++i) {
        DynamicMemory[last_index][i] = color;
    }
}

// Clear all patterns stored in DynamicMemory
void PixelCanvas::clearDynamicMemoryAll() {
    for (auto& pattern : DynamicMemory) {
        pattern.clear();
    }
    DynamicMemory.clear();
}

// Clear all patterns stored in PatternMemory
void PixelCanvas::clearPatternMemoryAll() {
    for (auto& pattern : PatternMemory) {
        pattern.clear();
    }
    PatternMemory.clear();
}

// Draw Pixels with coordinates on Real Canvas
void PixelCanvas::drawPixelsDynamic(int index,
                                    std::vector<int> pattern_idx, 
                                    uint32_t color, 
                                    bool use_parallel) {
    if (index >= (int) DynamicMemory.size()) {
        return;
    }
    std::vector<uint32_t>& PatternCanvas = DynamicMemory[index];
    #pragma omp parallel for if(use_parallel)
    for (size_t i = 0; i < pattern_idx.size(); ++i) {
        PatternCanvas[pattern_idx[i]] = color;
    }
}

// Draw Pixels with coordinates on Real Canvas with specified Bit Plane
void PixelCanvas::drawPixelsDynamicBit(int index, 
                                       int bit_plane, 
                                       std::vector<int> pattern_idx,
                                       bool color, 
                                       bool use_parallel) {
    if (index >= (int) DynamicMemory.size()) {
        return;
    }
    if (bit_plane < 0 || bit_plane > 23) {
        return;
    }
    std::vector<uint32_t>& PatternCanvas = DynamicMemory[index];
    #pragma omp parallel for if(use_parallel)
    for (size_t i = 0; i < pattern_idx.size(); ++i) {
        uint32_t mask = 1 << bit_plane;
        if (color) {
                PatternCanvas[pattern_idx[i]] |= mask;
        }
        else {
                PatternCanvas[pattern_idx[i]] &= ~mask;
        }
    }
}

// Project a black tweezer pattern with the specified number of tweezers
void PixelCanvas::generateBlackTweezerPattern(int num_tweezers, 
                                              const double *x0, 
                                              const double *y0, 
                                              double r, 
                                              double dx, 
                                              double dy,
                                              int num_RGB_buffers,
                                              int num_frames,
                                              bool use_parallel) {
    std::vector<int> start_indices = drawCirclesOnReal(num_tweezers, x0, y0, r, use_parallel);
    int num_RGB = (num_frames + 23) / 24;
    int num_residual = num_RGB * 24 - num_frames;
    DynamicMemory.resize(num_RGB_buffers + num_RGB);
    #pragma omp parallel for if(use_parallel)
    for (int i = 0; i < num_RGB_buffers; i++) {
        // Draw buffer frames
        DynamicMemory[i].resize(PatternNumPixels, 0xFFFFFFFF);
        drawPixelsDynamic(i, start_indices, 0xFF000000, use_parallel);
    }
    // Draw moving tweezers
    double * x = new double[num_tweezers];
    double * y = new double[num_tweezers];
    for (int j = 0; j < num_tweezers; j++) {
        x[j] = x0[j];
        y[j] = y0[j];
    }
    for (int i = 0; i < (num_frames + num_residual); i++) {
        if (i < num_frames) {
            for (int j = 0; j < num_tweezers; j++) {
                x[j] += dx;
                y[j] += dy;
            }
        }
        std::vector<int> new_indices = drawCirclesOnReal(num_tweezers, x, y, r, use_parallel);
        int RGB_index = num_RGB_buffers + i / 24;
        int bit_index = i % 24;
        int bit_plane;
        if (bit_index < 8) {
            // First 8 bits to Green
            bit_plane = bit_index + 8;
        } else if (bit_index < 16) {
            // Next 8 bits to Red
            bit_plane = bit_index + 8;
        } else {
            // Last 8 bits to Blue
            bit_plane = bit_index - 16;
        }
        // Draw the tweezers on the RGB buffer
        DynamicMemory[RGB_index].resize(PatternNumPixels, 0xFFFFFFFF);
        drawPixelsDynamicBit(RGB_index, bit_plane, new_indices, false, use_parallel);
    }
    delete[] x;
    delete[] y;
}

std::vector<int> PixelCanvas::drawCirclesOnReal(
                                    int num_circles,
                                    const double* x0, 
                                    const double* y0, 
                                    double r, 
                                    bool use_parallel) {
    const double r2 = r * r;
    std::vector<int> idx_to_draw;
    #pragma omp parallel if(use_parallel) 
    {
    std::vector<int> local_idx;
    #pragma omp for nowait
    for (int i = 0; i < num_circles; ++i) {
        const double cx = x0[i];
        const double cy = y0[i];
        const int x_min = std::max(0, int(cx - r));
        const int x_max = std::min(RealNumRows, int(cx + r) + 1);
        const int y_min = std::max(0, int(cy - r));
        const int y_max = std::min(RealNumCols, int(cy + r) + 1);
        for (int x = x_min; x < x_max; ++x) {
            const double dx2 = (x - cx) * (x - cx);
            for (int y = y_min; y < y_max; ++y) {
                const double dy2 = (y - cy) * (y - cy);
                if (dx2 + dy2 <= r2) {
                    int reali = sub2ind(RealNumRows, RealNumCols, x, y);
                    int patterni = Real2PatternIndex[reali];
                    if (patterni != -1) {
                        local_idx.push_back(patterni);    
                    }                    
                }
            }
        }
    }
    // Thread-safe appending of results
    #pragma omp critical
    idx_to_draw.insert(idx_to_draw.end(), local_idx.begin(), local_idx.end());
    }
    // Remove duplicates
    std::sort(idx_to_draw.begin(), idx_to_draw.end());
    idx_to_draw.erase(std::unique(idx_to_draw.begin(), idx_to_draw.end()), idx_to_draw.end());
    return idx_to_draw;
}

// Get Pattern Canvas in RGB format
std::vector<uint8_t> PixelCanvas::getDynamicMemoryRGB(int index, bool use_parallel) {
    if ((index < (int) DynamicMemory.size()) && (index >= 0)) {
        return convertPattern2RGB(DynamicMemory[index].data(), NumRows, NumCols, NumCols * 4, use_parallel);
    }
    return std::vector<uint8_t>();
}

// Get Real Canvas
std::vector<uint32_t> PixelCanvas::getDynamicMemoryReal(int index, uint32_t background_color, bool use_parallel) {
    if ((index < (int) DynamicMemory.size()) && (index >= 0)) {
        return convertPattern2Real(DynamicMemory[index].data(), background_color, use_parallel);
    }
    return std::vector<uint32_t>();
}

// Get Real Canvas in RGB format
std::vector<uint8_t> PixelCanvas::getDynamicMemoryRealRGB(int index, uint32_t background_color, bool use_parallel) {
    if ((index < (int) DynamicMemory.size()) && (index >= 0)) {
        std::vector<uint32_t> real_canvas = convertPattern2Real(DynamicMemory[index].data(), background_color, use_parallel);
        return convertPattern2RGB(real_canvas.data(), RealNumRows, RealNumCols, RealNumCols * 4, use_parallel);
    }
    return std::vector<uint8_t>();
}

// Get Pattern Memory in RGB format
std::vector<uint8_t> PixelCanvas::getPatternMemoryRGB(int index, bool use_parallel) {
    if ((index < (int) PatternMemory.size()) && (index >= 0)) {
        return convertPattern2RGB(PatternMemory[index].data(), NumRows, NumCols, NumCols * 4, use_parallel);
    }
    return std::vector<uint8_t>();
}

// Get Real Canvas from Pattern Memory
std::vector<uint32_t> PixelCanvas::getPatternMemoryReal(int index, uint32_t background_color, bool use_parallel) {
    if ((index < (int) PatternMemory.size()) && (index >= 0)) {
        return convertPattern2Real(PatternMemory[index].data(), background_color, use_parallel);
    }
    return std::vector<uint32_t>();
}

// Get Real Canvas in RGB format from Pattern Memory
std::vector<uint8_t> PixelCanvas::getPatternMemoryRealRGB(int index, uint32_t background_color, bool use_parallel) {
    if ((index < (int) PatternMemory.size()) && (index >= 0)) {
        std::vector<uint32_t> real_canvas = convertPattern2Real(PatternMemory[index].data(), background_color, use_parallel);
        return convertPattern2RGB(real_canvas.data(), RealNumRows, RealNumCols, RealNumCols * 4, use_parallel);
    }
    return std::vector<uint8_t>();
}

// Convert Pattern to Real Canvas, assuming pattern has the same size as the pattern canvas
std::vector<uint32_t> PixelCanvas::convertPattern2Real(const uint32_t* pattern, uint32_t background_color, bool use_parallel) {
    std::vector<uint32_t> real_canvas(RealNumPixels, background_color);
    #pragma omp parallel for if(use_parallel)
    for (int i = 0; i < PatternNumPixels; ++i) {
        real_canvas[Pattern2RealIndex[i]] = pattern[i];
    }
    return real_canvas;
}

// Convert Pattern to RGB (pixel takes 3 bytes in RGB format)
std::vector<uint8_t> PixelCanvas::convertPattern2RGB(const uint32_t *pattern, int height, int width, int pitch, bool use_parallel) {
    std::vector<uint8_t> rgb(3 * width * height);
    #pragma omp parallel for if(use_parallel)
    for (int i = 0; i < height; ++i) {
        for (int j = 0; j < width; ++j) {
            int idx = i * width + j;
            uint32_t pixel = pattern[idx];
            rgb[3 * idx + 0] = (pixel >> 16) & 0xFF;  // R
            rgb[3 * idx + 1] = (pixel >> 8)  & 0xFF;  // G
            rgb[3 * idx + 2] = (pixel >> 0)  & 0xFF;  // B
        }
    }
    return rgb;
}

// Convert RGB to Pattern (uint32_t 4 bytes format)
std::vector<uint32_t> PixelCanvas::convertRGB2Pattern(const uint8_t *rgb, int height, int width, int pitch, bool use_parallel) {
    std::vector<uint32_t> pattern(width * height);
    #pragma omp parallel for if(use_parallel)
    for (int i = 0; i < height; ++i) {
        for (int j = 0; j < width; ++j) {
            int idx = i * width + j;
            uint8_t r = rgb[3 * idx + 0];
            uint8_t g = rgb[3 * idx + 1];
            uint8_t b = rgb[3 * idx + 2];
            pattern[idx] = (0xFF << 24) | (r << 16) | (g << 8) | b;
        }
    }
    return pattern;
}
