#include <PixelCanvas.h>

PixelCanvas::~PixelCanvas() {
    closeCanvas();
}

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
    else {
        throw std::invalid_argument("Invalid pixel arrangement.");
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
    // Assign background to real index mapping
    BackgroundNumPixels = RealNumPixels - PatternNumPixels;
    Background2RealIndex.resize(BackgroundNumPixels);
    int bg_idx = 0;
    for (int i = 0; i < RealNumPixels; ++i) {
        if (Real2PatternIndex[i] == -1) {
            Background2RealIndex[bg_idx++] = i;
        }
    }
    PatternCanvas.resize(PatternNumPixels, 0);
    RealCanvas.resize(RealNumPixels, 0);
    resetBackground();
    resetPattern();
}

// Reset Background
void PixelCanvas::resetBackground(uint32_t background_color, bool use_parallel) {
    #pragma omp parallel for if(use_parallel)
    for (int i = 0; i < BackgroundNumPixels; ++i) {
        RealCanvas[Background2RealIndex[i]] = background_color;
    }
}

// Reset Pattern
void PixelCanvas::resetPattern(uint32_t pattern_color, bool use_parallel) {
    #pragma omp parallel for if(use_parallel)
    for (int i = 0; i < PatternNumPixels; ++i) {
        RealCanvas[Pattern2RealIndex[i]] = pattern_color;
        PatternCanvas[i] = pattern_color;
    }
}

// Clear Pattern Memory
void PixelCanvas::clearPatternMemory() {
    for (auto& pattern : PatternMemory) {
        pattern.clear();
    }
    PatternMemory.clear();
}

// Load a pattern to PatternMemory
void PixelCanvas::loadPatternMemory(const uint32_t *pattern, size_t num_elements) {
    PatternMemory.push_back(std::vector<uint32_t>(pattern, pattern + num_elements));
}

// Cleanup Function
void PixelCanvas::closeCanvas() {
    PixelArrangement.clear();
    Pattern2RealIndex.clear();
    Background2RealIndex.clear();
    Real2PatternIndex.clear();
    PatternCanvas.clear();
    RealCanvas.clear();
    clearPatternMemory();
    NumRows = 0;
    NumCols = 0;
    RealNumRows = 0;
    RealNumCols = 0;
    PatternNumPixels = 0;
    BackgroundNumPixels = 0;
    RealNumPixels = 0;
}

// Update Pattern to Real
void PixelCanvas::updatePattern2Real(bool use_parallel) {
    #pragma omp parallel for if(use_parallel)
    for (size_t i = 0; i < PatternCanvas.size(); ++i) {
        PatternCanvas[i] = RealCanvas[Pattern2RealIndex[i]];
    }
}

// Update Real to Pattern
void PixelCanvas::updateReal2Pattern(bool use_parallel) {
    #pragma omp parallel for if(use_parallel)
    for (size_t i = 0; i < PatternCanvas.size(); ++i) {
        RealCanvas[Pattern2RealIndex[i]] = PatternCanvas[i];
    }
}

// Get Pattern Canvas in RGB format
std::vector<uint8_t> PixelCanvas::getPatternCanvasRGB(bool use_parallel) {
    return convertPattern2RGB((uint8_t *) PatternCanvas.data(), NumRows, NumCols, NumCols * 4, use_parallel);
}

// Get Real Canvas in RGB format
std::vector<uint8_t> PixelCanvas::getRealCanvasRGB(bool use_parallel) {
    return convertPattern2RGB((uint8_t *) RealCanvas.data(), RealNumRows, RealNumCols, RealNumCols * 4, use_parallel);
}

// Convert Pattern to RGB (pixel takes 3 bytes in RGB format)
std::vector<uint8_t> PixelCanvas::convertPattern2RGB(uint8_t *pattern, int height, int width, int pitch, bool use_parallel) {
    std::vector<uint8_t> rgb(3 * width * height);
    #pragma omp parallel for if(use_parallel)
    for (int i = 0; i < height; ++i) {
        for (int j = 0; j < width; ++j) {
            int pixel_index = i * pitch + 4 * j;
            int rgb_index = 3 * (i * width + j);
            rgb[rgb_index] = pattern[pixel_index + 2]; // R
            rgb[rgb_index + 1] = pattern[pixel_index + 1]; // G
            rgb[rgb_index + 2] = pattern[pixel_index]; // B
        }
    }
    return rgb;
}

// Convert RGB to Pattern (uint32_t 4 bytes format)
std::vector<uint32_t> PixelCanvas::convertRGB2Pattern(uint8_t *rgb, int height, int width, int pitch, bool use_parallel) {
    std::vector<uint32_t> pattern(width * height);
    #pragma omp parallel for if(use_parallel)
    for (int i = 0; i < height; ++i) {
        for (int j = 0; j < width; ++j) {
            int pixel_index = i * pitch + 3 * j;
            int pattern_index = i * width + j;
            pattern[pattern_index] = (0xFF000000) | (rgb[pixel_index] << 16) | (rgb[pixel_index + 1] << 8) | rgb[pixel_index + 2];
        }
    }
    return pattern;
}
