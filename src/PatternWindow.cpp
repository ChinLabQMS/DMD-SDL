#include "PatternWindow.h"

PatternWindow::PatternWindow() {
    init(false); // Initialize SDL for window management upon object creation
}

// Every time the window is opened, the canvas is initialized
void PatternWindow::open(std::string arrangement, bool use_parallel) {
    BaseWindow::open(false);
    if (Window) {
        initCanvas(WindowHeight, WindowWidth, arrangement);
    }
}

// Close the window and free canvas resources
void PatternWindow::close() {
    BaseWindow::close(false);
    closeCanvas();
}

void PatternWindow::setStaticPatternPath(const char *filepath, bool use_parallel) {
    BaseWindow::setStaticPatternPath(filepath, false);
    if (StaticPatternSurface) {
        StaticPatternRGB = convertPattern2RGB((uint8_t *) StaticPatternSurface->pixels, 
                                              StaticPatternSurface->h, 
                                              StaticPatternSurface->w, 
                                              StaticPatternSurface->pitch, 
                                              use_parallel);
    }
    else {
        StaticPatternRGB.clear();
    }
}
