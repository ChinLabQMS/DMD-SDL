#include "PatternWindow.h"

void PatternWindow::open(bool verbose) {
    error("Please use the open2 function with additional parameters for canvas initialization.");
}

// Every time the window is opened, the canvas is initialized
void PatternWindow::open2(std::string arrangement, bool verbose, bool use_parallel) {
    BaseWindow::open(verbose);
    initCanvas(WindowHeight, WindowWidth, arrangement, use_parallel);
    if (StaticPatternPath) {
        char *path = SDL_strdup(StaticPatternPath);
        setStaticPatternPath(path, verbose);
        SDL_free(path);
    }
}

// Close the window and free canvas resources; override the BaseWindow close function
void PatternWindow::close(bool verbose) {
    BaseWindow::close(verbose);
    closeCanvas();
}

// Set the path to the static pattern image file; override the BaseWindow setStaticPatternPath function
void PatternWindow::setStaticPatternPath(const char *filepath, bool verbose) {
    setStaticPatternPath2(filepath, verbose, true);
}

// Set the path to the static pattern image file with an additional parameter for parallel processing
void PatternWindow::setStaticPatternPath2(const char *filepath, bool verbose, bool use_parallel) {
    BaseWindow::setStaticPatternPath(filepath, verbose);
    if (StaticPatternSurface) {
        StaticPatternRGB = convertPattern2RGB(
            (uint8_t *) StaticPatternSurface->pixels, 
            StaticPatternSurface->h,
            StaticPatternSurface->w,
            StaticPatternSurface->pitch, 
            use_parallel);
        StaticPatternReal = convertPattern2Real(
            (uint32_t *) StaticPatternSurface->pixels, 
            0xFFFF0000, use_parallel);
        StaticPatternRealRGB = convertPattern2RGB(
            (uint8_t *) StaticPatternReal.data(), 
            RealNumRows, 
            RealNumCols, 
            RealNumCols * 4,
            use_parallel);
    }
    else {
        StaticPatternRGB.clear();
        StaticPatternReal.clear();
        StaticPatternRealRGB.clear();
    }
}

// Load a pattern memory from a file
void PatternWindow::loadPatternMemoryFromFile(const char *filepath, bool verbose, bool use_parallel) {
    readBMP(filepath, NULL, false);
    if (BMPSurface) {
        if (BMPSurface->h * BMPSurface->w == PatternNumPixels) {
            uint32_t *pixels = (uint32_t*) BMPSurface->pixels;
            PatternMemory.push_back(std::vector<uint32_t>(pixels, pixels + BMPSurface->h * BMPSurface->w));
            if (verbose) {
                printf("Pattern memory loaded from file: %s", filepath);
            }
        } else {
            warn("Pattern size does not match the canvas size, skipping file: %s", filepath);
        }
    }
}

// Select files from the file system and load them into the pattern memory
void PatternWindow::selectAndLoadPatternMemory(const char *default_location, bool verbose, bool use_parallel) {
    if (!default_location) {
        default_location = BaseDirectory;
    }
    SDL_ShowOpenFileDialog(callback, this, NULL, filters, 2, default_location, true);
    // Delay the return until the user input is received
    SDL_Event event;
    while (true) {
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_EVENT_USER) {
                if (event.user.code == -1) {
                    error("An error occured during file selection: %s", SDL_GetError());
                }
                if (event.user.code == 1) {
                    warn("The user did not select any file.");
                }
                if (event.user.code == 0) {
                    for (int i = 0; i < *((int*) event.user.data1); i++) {
                        loadPatternMemoryFromFile(((char**) event.user.data2)[i], verbose, use_parallel);
                    }
                }
                return;
            }
        }
    }
}

// Display a pattern from the pattern memory
// If the delay is greater than 0, the function will wait for the specified time before returning
void PatternWindow::displayPatternMemory(int index, uint32_t delay, bool verbose, bool use_parallel) {
    if (!Window) {
        error("Window is not open.");
        return;
    }
    if ((index < 0) | (index >= PatternMemory.size())) {
        error("Invalid pattern memory index.");
        return;
    }
    setDynamicPattern(PatternMemory[index].data(), verbose, use_parallel);
    if (delay > 0) {
        SDL_Delay(delay);
    }
}
