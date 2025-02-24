#include <PixelCanvas.h>

PixelCanvas::~PixelCanvas() {
    closeCanvas();
}

// Update array a with values from array c at indices specified by array b
void updateArray(int* a, int N, const int* b, const int* c, int M) {
    #pragma omp parallel for
    for (int i = 0; i < M; ++i) {
        if (b[i] >= 0 && b[i] < N) {
            a[b[i]] = c[i];
        }
    }
}

// Convert subscripts to linear indices, use C-style indexing
int sub2ind(int nrows, int ncols, int x, int y) {
    return x * ncols + y;
}

void PixelCanvas::initCanvas(int nrows, int ncols, const std::string arrangement) {
    PixelArrangement = arrangement;
    NumRows = nrows;
    NumCols = ncols;

    // Allocate space for PixelIndex
    PixelCount = nrows * ncols;
    PixelIndex = new int[PixelCount];

    // Compute projector space indices, use C-style indexing
    for (int i = 0; i < PixelCount; ++i) {
        PixelIndex[i] = i;
    }

    // Handle different pixel arrangements
    if (PixelArrangement == "Square") {
        RealNumRows = NumRows;
        RealNumCols = NumCols;
        RealPixelCount = PixelCount;
        RealPixelIndex = new int[PixelCount];
        for (int i = 0; i < PixelCount; ++i) {
            RealPixelIndex[i] = PixelIndex[i];
        }
        BackgroundCount = 0;
        RealBackgroundIndex = NULL;
    }
    else if (PixelArrangement == "Diamond") {
        RealNumRows = std::max(0, nrows / 2 + ncols);
        RealNumCols = std::max(0, ncols + (nrows - 1) / 2);
        RealPixelCount = RealNumRows * RealNumCols;
        
        // Allocate space for RealPixelIndex
        RealPixelIndex = new int[PixelCount];
    
        // Compute transformed coordinates directly with integer division
        for (int i = 0; i < PixelCount; ++i) {
            int x = i / ncols;  // row index
            int y = i % ncols;  // column index        
            int real_x = (nrows - x) / 2 + y;
            int real_y = (nrows - 1 - x) / 2 + ncols - y - 1;
            RealPixelIndex[i] = sub2ind(RealNumRows, RealNumCols, real_x, real_y);
        }

        // Compute background indices
        BackgroundCount = RealPixelCount - PixelCount;
        RealBackgroundIndex = NULL;
        // Memory leak
        // if (BackgroundCount != 0) {
        //     bool *real_idx = new bool[RealPixelCount];
        //     for (int i = 0; i < RealPixelCount; ++i) {
        //         real_idx[i] = true;
        //     }
        //     for (int i = 0; i < PixelCount; ++i) {
        //         real_idx[RealPixelIndex[i]] = false;
        //     }
        //     RealBackgroundIndex = new int[BackgroundCount];
        //     int bg_index = 0;
        //     for (int i = 0; i < RealPixelCount; ++i) {
        //         if (!real_idx[i]) {
        //             RealBackgroundIndex[bg_index++] = i;
        //         }
        //     }
        //     delete[] real_idx;
        // }
    }

    // Allocate space for PatternCanvas and RealPatternCanvas
    PatternCanvas = new uint32_t[PixelCount];
    RealPatternCanvas = new uint32_t[RealPixelCount];
    resetCanvas();
}

void PixelCanvas::resetCanvas(uint32_t pattern_color, uint32_t background_color) {
    for (int i = 0; i < PixelCount; ++i) {
        PatternCanvas[i] = 0;
    }
    for (int i = 0; i < RealPixelCount; ++i) {
        RealPatternCanvas[i] = background_color;
    }
    for (int i = 0; i < PixelCount; ++i) {
        RealPatternCanvas[RealPixelIndex[i]] = pattern_color;
    }
}

void PixelCanvas::closeCanvas() {
    delete[] PixelIndex;
    delete[] RealPixelIndex;
    delete[] RealBackgroundIndex;
    delete[] PatternCanvas;
    delete[] RealPatternCanvas;
}

void PixelCanvas::updatePattern2Real() {
    for (int i = 0; i < PixelCount; ++i) {
        PatternCanvas[i] = RealPatternCanvas[RealPixelIndex[i]];
    }
}

void PixelCanvas::updateReal2Pattern() {
    for (int i = 0; i < PixelCount; ++i) {
        RealPatternCanvas[RealPixelIndex[i]] = PatternCanvas[i];
    }
}

void PixelCanvas::copyPixel2Pattern(uint32_t *pixels) {
    for (int i = 0; i < PixelCount; ++i) {
        PatternCanvas[i] = pixels[i];
    }
}

void PixelCanvas::copyPixel2Real(uint32_t *pixels) {
    for (int i = 0; i < RealPixelCount; ++i) {
        RealPatternCanvas[i] = pixels[i];
    }
}
