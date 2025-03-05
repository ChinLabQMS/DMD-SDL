#include "PatternWindow.h"

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

void PatternWindow::loadPatternMemoryFromFile(const char *filepath, bool use_parallel) {
    readBMP(filepath, NULL, false);
    if (BMPSurface) {
        loadPatternMemory((uint32_t *) BMPSurface->pixels, BMPSurface->h * BMPSurface->w);
    }
}

void PatternWindow::displayPatternMemory(bool use_parallel) {
    if (!Window) {
        error("Window is not open.");
        return;
    } else {
        for (size_t i = 0; i < PatternMemory.size(); ++i) {
            setDynamicPattern(PatternMemory[i].data(), false, use_parallel);
        }
    }
}
