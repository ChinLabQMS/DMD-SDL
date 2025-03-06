#include "PatternWindow.h"

PatternWindow::PatternWindow(std::string arrangement) : BaseWindow(), PixelCanvas(arrangement) {}

// Override the BaseWindow open function
void PatternWindow::open(bool verbose) {
    BaseWindow::open(verbose);
    if (Window) {
        initCanvas(WindowHeight, WindowWidth, true);
    }
}

// Every time the window is opened, the canvas is initialized
void PatternWindow::open(bool verbose, bool use_parallel) {
    BaseWindow::open(verbose);
    if (Window) {
        initCanvas(WindowHeight, WindowWidth, use_parallel);
    }
}

// Close the window and free canvas resources; override the BaseWindow close function
void PatternWindow::close(bool verbose) {
    BaseWindow::close(verbose);
    closeCanvas();
}

// Set the path to the static pattern image file; override the BaseWindow setStaticPatternPath function
void PatternWindow::setStaticPatternPath(const char *filepath, bool verbose) {
    this->setStaticPatternPath(filepath, verbose, true);
}

// Set the path to the static pattern image file with an additional parameter for parallel processing
void PatternWindow::setStaticPatternPath(const char *filepath, bool verbose, bool use_parallel) {
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

// Load a pattern memory from an array of pixel data, override the PixelCanvas loadPatternMemory function
void PatternWindow::loadPatternMemory(const uint32_t *pattern, size_t num_elements) {
    if (num_elements != PatternNumPixels) {
        error("Pattern size (%d) does not match canvas size (%d)!", num_elements, PatternNumPixels);
        return;
    }
}

// Select files from the file system and load them into the pattern memory
void PatternWindow::selectAndLoadPatternMemory(const char *default_location, bool verbose, bool use_parallel) {
    selectAndReadBMP(default_location, verbose);
    if (BMPSurface) {
        loadPatternMemory((uint32_t *) BMPSurface->pixels, BMPSurface->h * BMPSurface->w);
    }
}

// Load a pattern memory from a file
void PatternWindow::loadPatternMemoryFromFile(const char *filepath, bool verbose, bool use_parallel) {
    readBMP(filepath, NULL, verbose);
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
