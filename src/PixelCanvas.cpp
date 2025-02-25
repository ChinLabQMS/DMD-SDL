#include <PixelCanvas.h>

PixelCanvas::PixelCanvas(std::string arrangement) : PixelArrangement(std::move(arrangement)) {}

PixelCanvas::~PixelCanvas() {
    closeCanvas();
}

// Parallelized array updates
void updateArrayFast(std::vector<uint32_t>& a, const std::vector<int>& idx, const std::vector<uint32_t>& val) {
    int N = a.size();
    int M = idx.size();
    #pragma omp parallel for
    for (int i = 0; i < M; ++i) {
        if (idx[i] >= 0 && idx[i] < N) {
            a[idx[i]] = val[i];
        }
    }
}

void updateArray(std::vector<uint32_t>& a, const std::vector<int>& idx, const std::vector<uint32_t>& val) {
    int N = a.size();
    int M = idx.size();
    for (int i = 0; i < M; ++i) {
        if (idx[i] >= 0 && idx[i] < N) {
            a[idx[i]] = val[i];
        }
    }
}

void updateArrayReverseFast(const std::vector<uint32_t>& a, const std::vector<int>& idx, std::vector<uint32_t>& val) {
    int N = a.size();
    int M = idx.size();
    #pragma omp parallel for
    for (int i = 0; i < M; ++i) {
        if (idx[i] >= 0 && idx[i] < N) {
            val[i] = a[idx[i]];
        }
    }
}

void updateArrayReverse(const std::vector<uint32_t>& a, const std::vector<int>& idx, std::vector<uint32_t>& val) {
    int N = a.size();
    int M = idx.size();
    for (int i = 0; i < M; ++i) {
        if (idx[i] >= 0 && idx[i] < N) {
            val[i] = a[idx[i]];
        }
    }
}

// Convert subscripts to linear indices, use C-style indexing
int sub2ind(int nrows, int ncols, int x, int y) {
    return x * ncols + y;
}

// Initialize Canvas
void PixelCanvas::initCanvas(int nrows, int ncols) {
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
void PixelCanvas::resetCanvas(uint32_t pattern_color, uint32_t background_color) {
    std::fill(PatternCanvas.begin(), PatternCanvas.end(), pattern_color);
    for (int i = 0; i < BackgroundCount; ++i) {
        RealPatternCanvas[RealBackgroundIndex[i]] = background_color;
    }
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
void PixelCanvas::updatePattern2Real(bool fast) {
    if (fast)
        updateArrayReverseFast(RealPatternCanvas, RealPixelIndex, PatternCanvas);
    else
        updateArrayReverse(RealPatternCanvas, RealPixelIndex, PatternCanvas);
}

// Update from Real to Pattern
void PixelCanvas::updateReal2Pattern(bool fast) {
    if (fast)
        updateArrayFast(RealPatternCanvas, RealPixelIndex, PatternCanvas);
    else
        updateArray(RealPatternCanvas, RealPixelIndex, PatternCanvas);
}

// Copy Pixels to Pattern using raw pointer
void PixelCanvas::copyPixel2Pattern(uint32_t* pixels, size_t num_pixels) {
    std::copy(pixels, pixels + num_pixels, PatternCanvas.begin());
}

// Copy Pixels to Real using raw pointer
void PixelCanvas::copyPixel2Real(uint32_t* pixels, size_t num_pixels) {
    std::copy(pixels, pixels + num_pixels, RealPatternCanvas.begin());
}
