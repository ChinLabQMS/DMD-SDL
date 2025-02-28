#include "PatternWindow.h"

PatternWindow::PatternWindow() {
    init(false); // Initialize SDL
}

// Every time the window is opened, the canvas is initialized
void PatternWindow::open(std::string arrangement = "Diamond", bool use_parallel = true) {
    BaseWindow::open(false);
    if (Window) {
        initCanvas(WindowHeight, WindowWidth, arrangement);
        updateCanvas2StaticPattern(use_parallel);
    }
}

void PatternWindow::close() {
    BaseWindow::close(false);
    closeCanvas();
}

void PatternWindow::setStaticPatternPath(const char *filepath, bool use_parallel = true) {
    BaseWindow::setStaticPatternPath(filepath, false);
    updateCanvas2StaticPattern(use_parallel);
}

void PatternWindow::projectDynamicPatternFromCanvas() {
    if (PatternCanvas.size() == WindowWidth * WindowHeight) {
        setDynamicPattern((void*) PatternCanvas.data(), false);
    }
    else {
        error("Invalid pattern size, number of elements is %d, while window size is (%d, %d).", 
              PatternCanvas.size(), WindowWidth, WindowHeight);
    }
}

void PatternWindow::updateCanvas2StaticPattern(bool use_parallel = true) {
    if (StaticPatternSurface) {
        copyPixel2Pattern((uint32_t*) StaticPatternSurface->pixels, 
                          StaticPatternSurface->w * StaticPatternSurface->h, 
                          use_parallel);
        updateReal2Pattern(use_parallel);
    }
}

void PatternWindow::updateCanvas2DynamicPattern(bool use_parallel = true) {
    if (DynamicPatternSurface) {
        copyPixel2Pattern((uint32_t*) DynamicPatternSurface->pixels, 
                          DynamicPatternSurface->w * DynamicPatternSurface->h, 
                          use_parallel);
        updateReal2Pattern(use_parallel);
    }
}