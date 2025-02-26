#include <PixelCanvas.h>


PixelCanvas::~PixelCanvas() {
    closeCanvas();
}

// Convert subscripts to linear indices, use C-style indexing
int sub2ind(int nrows, int ncols, int x, int y) {
    return x * ncols + y;
}

// Initialize Canvas
void PixelCanvas::initCanvas(int nrows, int ncols, std::string arrangement) {
    PixelArrangement = std::move(arrangement);
    IsCanvasInitialized = true;
    NumRows = nrows;
    NumCols = ncols;
    PixelCount = NumRows * NumCols;

    PixelIndex.resize(PixelCount);
    std::iota(PixelIndex.begin(), PixelIndex.end(), 0);  // Fill with sequential indices

    if (PixelArrangement == "Square") {
        RealNumRows = NumRows;
        RealNumCols = NumCols;
        RealPixelCount = PixelCount;
        RealPixelIndex = PixelIndex;
        BackgroundCount = 0;
    }
    else if (PixelArrangement == "Diamond") {
        RealNumRows = std::max(0, nrows / 2 + ncols);
        RealNumCols = std::max(0, ncols + (nrows - 1) / 2);
        RealPixelCount = RealNumRows * RealNumCols;
        RealPixelIndex.resize(PixelCount);

        for (int i = 0; i < PixelCount; ++i) {
            int x = i / ncols;  // row index
            int y = i % ncols;  // column index        
            int real_x = (nrows - x) / 2 + y;
            int real_y = (nrows - 1 - x) / 2 + ncols - y - 1;
            RealPixelIndex[i] = sub2ind(RealNumRows, RealNumCols, real_x, real_y);
        }

        BackgroundCount = RealPixelCount - PixelCount;
        if (BackgroundCount != 0) {
            std::vector<bool> real_idx(RealPixelCount, true);
            for (int idx : RealPixelIndex) {
                real_idx[idx] = false;
            }
            RealBackgroundIndex.clear();
            for (int i = 0; i < RealPixelCount; ++i) {
                if (!real_idx[i]) {
                    RealBackgroundIndex.push_back(i);
                }
            }
        }
    }

    PatternCanvas.resize(PixelCount, 0);
    RealPatternCanvas.resize(RealPixelCount, 0);
    resetCanvas();
}

// Reset Canvas
void PixelCanvas::resetCanvas(uint32_t pattern_color, uint32_t background_color, bool use_parallel) {
    std::fill(PatternCanvas.begin(), PatternCanvas.end(), pattern_color);
    #pragma omp parallel for if(use_parallel)
    for (int i = 0; i < BackgroundCount; ++i) {
        RealPatternCanvas[RealBackgroundIndex[i]] = background_color;
    }
    #pragma omp parallel for if(use_parallel)
    for (int i = 0; i < PixelCount; ++i) {
        RealPatternCanvas[RealPixelIndex[i]] = pattern_color;
    }
}

// Cleanup Function
void PixelCanvas::closeCanvas() {
    PixelIndex.clear();
    RealPixelIndex.clear();
    RealBackgroundIndex.clear();
    PatternCanvas.clear();
    RealPatternCanvas.clear();
}

// Update from Pattern to Real
void PixelCanvas::updatePattern2Real(bool use_parallel) {
    #pragma omp parallel for if(use_parallel)
    for (size_t i = 0; i < PatternCanvas.size(); ++i) {
        PatternCanvas[i] = RealPatternCanvas[RealPixelIndex[i]];
    }
}

// Update from Real to Pattern
void PixelCanvas::updateReal2Pattern(bool use_parallel) {
    #pragma omp parallel for if(use_parallel)
    for (size_t i = 0; i < PatternCanvas.size(); ++i) {
        RealPatternCanvas[RealPixelIndex[i]] = PatternCanvas[i];
    }
}

// Copy Pixels to Pattern using raw pointer
void PixelCanvas::copyPixel2Pattern(uint32_t* pixels, size_t num_pixels, bool use_parallel) {
    std::copy(pixels, pixels + num_pixels, PatternCanvas.begin());
    updateReal2Pattern(use_parallel);
}

// Copy Pixels to Real using raw pointer
void PixelCanvas::copyPixel2Real(uint32_t* pixels, size_t num_pixels, bool use_parallel) {
    std::copy(pixels, pixels + num_pixels, RealPatternCanvas.begin());
    updatePattern2Real(use_parallel);
}

// Convert Pattern to RGB
// Function for raw pointer
std::vector<uint8_t> PixelCanvas::convertPattern2RGB(const uint32_t *pixels, size_t num_elements, bool use_parallel) {
    std::vector<uint8_t> rgb(num_elements * 3);
    // Enable parallel processing if requested
    #pragma omp parallel for if(use_parallel)
    for (size_t i = 0; i < num_elements; ++i) {
        rgb[i] = pixels[i] & 0xFF;                     // Red
        rgb[i + num_elements] = (pixels[i] >> 8) & 0xFF; // Green
        rgb[i + 2 * num_elements] = (pixels[i] >> 16) & 0xFF; // Blue
    }
    return rgb;
}

// Convert RGB to Pattern
std::vector<uint32_t> PixelCanvas::convertRGB2Pattern(const uint8_t *rgb, size_t num_elements, bool alpha_mask, bool use_parallel) {
    std::vector<uint32_t> pattern;
    pattern.resize(num_elements / 3);
    for (size_t i = 0; i < pattern.size(); ++i) {
        pattern[i] = (rgb[i + 2 * pattern.size()] << 16) + (rgb[i + pattern.size()] << 8) + rgb[i] + (alpha_mask ? 0xFF000000 : 0);
    }
    return pattern;
}
