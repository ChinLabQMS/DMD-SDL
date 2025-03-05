#include "PatternWindowMex.h"

MexFunction::MexFunction() {
    init(false); // Initialize SDL upon creation
}

// Overloaded printf function to display messages in MATLAB console
void MexFunction::printf(const char* format, ...) {
    char buffer[256];
    va_list args;
    va_start(args, format);
    vsprintf(buffer, format, args);
    va_end(args);
    matlab->feval(u"fprintf", 0, std::vector<Array>({ factory.createScalar(buffer) }));
    matlab->feval(u"fprintf", 0, std::vector<Array>({ factory.createScalar("\n") }));
}

// Overloaded warn function to display messages in MATLAB console
void MexFunction::warn(const char* format, ...) {
    char buffer[256];
    va_list args;
    va_start(args, format);
    vsprintf(buffer, format, args);
    va_end(args);
    matlab->feval(u"warning", 0, std::vector<Array>({ factory.createScalar(buffer) }));
}

// Overloaded printf function to display messages in MATLAB console
void MexFunction::error(const char* format, ...) {
    char buffer[256];
    va_list args;
    va_start(args, format);
    vsprintf(buffer, format, args);
    va_end(args);
    matlab->feval(u"error", 0, std::vector<Array>({ factory.createScalar(buffer) }));
}

// Lock the MEX function to prevent it from being cleared from memory when clearing mex
void MexFunction::lock() {
    if (!LockState) {
        mexLock();
        LockState = true;
    }
}

// Unlock the MEX function to allow it to be cleared from memory when clearing mex
void MexFunction::unlock() {
    if (LockState) {
        mexUnlock();
        LockState = false;
    }
}

// Set the path to the static pattern image file
void MexFunction::setStaticPatternPath(const char* filepath, bool use_parallel) {
    PatternWindow::setStaticPatternPath(filepath, use_parallel);
    // Update the Matlab array of the static pattern
    if (StaticPatternSurface) {
        uint32_t * pixels = (uint32_t*) StaticPatternSurface->pixels;
        StaticPatternMex = factory.createArray({ (size_t) StaticPatternSurface->h, (size_t) StaticPatternSurface->w }, 
                                                pixels, 
                                                pixels + StaticPatternSurface->h * StaticPatternSurface->w, 
                                                InputLayout::ROW_MAJOR);
        StaticPatternRGBMex = factory.createArray(
            { (size_t) WindowHeight, (size_t) WindowWidth, 3 }, 
            StaticPatternRGB.begin(), StaticPatternRGB.end(), 
            InputLayout::ROW_MAJOR);
    }
    else {
        StaticPatternMex = factory.createArray({0, 0}, (uint32_t *) nullptr, (uint32_t *) nullptr);
        StaticPatternRGBMex = factory.createArray({0, 0, 3}, (uint8_t *) nullptr, (uint8_t *) nullptr);
    }
}

// Get the operation mode of the window
CharArray MexFunction::getOperationMode() {
    if (OperationMode) {
        return factory.createCharArray(OperationMode);
    } else {
        return factory.createArray({0, 0}, (char *) nullptr, (char *) nullptr);
    }
}

CharArray MexFunction::getBaseDirectory() {
    if (BaseDirectory) {
        return factory.createCharArray(BaseDirectory);
    } else {
        return factory.createArray({0, 0}, (char *) nullptr, (char *) nullptr);
    }
}

// Get the path to the static pattern image file
CharArray MexFunction::getStaticPatternPath() {
    if (StaticPatternPath) {
        return factory.createCharArray(StaticPatternPath);
    } else {
        return factory.createArray({0, 0}, (char *) nullptr, (char *) nullptr);
    }
}

// Get the display modes of all connected displays as MATLAB struct array
StructArray MexFunction::getDisplayModes() {
    int num_displays;
    SDL_DisplayID *displays = SDL_GetDisplays(&num_displays);
    StructArray display_modes = factory.createStructArray({ (size_t) num_displays }, 
                            { "Width", "Height", "RefreshRate" });
    for (int i = 0; i < num_displays; i++) {
        const SDL_DisplayMode *mode = SDL_GetDesktopDisplayMode(displays[i]);
        display_modes[i]["Width"] = factory.createScalar(mode->w);
        display_modes[i]["Height"] = factory.createScalar(mode->h);
        display_modes[i]["RefreshRate"] = factory.createScalar(mode->refresh_rate);
    }
    return display_modes;
}

TypedArray<uint32_t> MexFunction::getBMPSurfacePattern() {
    if (BMPSurface) {
        return factory.createArray({ (size_t) BMPSurface->h, (size_t) BMPSurface->w }, 
                                    (uint32_t *) BMPSurface->pixels, 
                                    (uint32_t *) BMPSurface->pixels + BMPSurface->h * BMPSurface->w, 
                                    InputLayout::ROW_MAJOR);
    } else {
        return factory.createArray({0, 0}, (uint32_t *) nullptr, (uint32_t *) nullptr);
    }
}

// Get the pattern canvas as a MATLAB array
TypedArray<uint32_t> MexFunction::getPatternCanvas() {
    if (PatternCanvas.size() > 0) {
        return factory.createArray({ (size_t) WindowHeight, (size_t) WindowWidth }, 
                                    PatternCanvas.begin(), PatternCanvas.end(), 
                                    InputLayout::ROW_MAJOR);
    } else {
        return factory.createArray({0, 0}, (uint32_t *) nullptr, (uint32_t *) nullptr);
    }
}

// Get the pattern canvas as a MATLAB RGB array
TypedArray<uint8_t> MexFunction::getPatternCanvasRGB(bool use_parallel) {
    if (PatternCanvas.size() > 0) {
        std::vector<uint8_t> pattern_rgb = PixelCanvas::getPatternCanvasRGB();
        return factory.createArray({ (size_t) WindowHeight, (size_t) WindowWidth, 3 }, 
                                    pattern_rgb.begin(), pattern_rgb.end(), 
                                    InputLayout::ROW_MAJOR);
    } else {
        return factory.createArray({0, 0, 3}, (uint8_t *) nullptr, (uint8_t *) nullptr);
    }
}

// Get the real canvas as a MATLAB array
TypedArray<uint32_t> MexFunction::getRealCanvas() {
    if (RealCanvas.size() > 0) {
        return factory.createArray({ (size_t) RealNumRows, (size_t) RealNumCols }, 
                                    RealCanvas.begin(), RealCanvas.end(), 
                                    InputLayout::ROW_MAJOR);
    } else {
        return factory.createArray({0, 0}, (uint32_t *) nullptr, (uint32_t *) nullptr);
    }
}

// Get the real canvas as a MATLAB RGB array
TypedArray<uint8_t> MexFunction::getRealCanvasRGB(bool use_parallel) {
    if (RealCanvas.size() > 0) {
        std::vector<uint8_t> real_rgb = PixelCanvas::getRealCanvasRGB(use_parallel);
        return factory.createArray({ (size_t) RealNumRows, (size_t) RealNumCols, 3 }, 
                                    real_rgb.begin(), real_rgb.end(), 
                                    InputLayout::ROW_MAJOR);
    } else {
        return factory.createArray({0, 0, 3}, (uint8_t *) nullptr, (uint8_t *) nullptr);
    }
}

// Check the arguments of the MEX function
void MexFunction::checkArguments(ArgumentList outputs, ArgumentList inputs) {
    // Check number of arguments: if empty, assume init function
    if (inputs.size() == 0) {
        return;
    }
    // Check first argument: First input must be scalar string that specifies the function to call
    if (inputs[0].getType() != ArrayType::MATLAB_STRING || inputs[0].getNumberOfElements() != 1){
        error("First input must be a scalar string.");
    }
}

// Convert pattern to RGB, assuming column major order in pattern
TypedArray<uint8_t> MexFunction::convertPattern2RGBMex(const TypedArray<uint32_t> pattern, bool use_parallel) {
    uint32_t *pattern_ptr = (uint32_t *) getDataPtr<uint32_t>(pattern);
    size_t height = pattern.getDimensions()[0];
    size_t width = pattern.getDimensions()[1];
    std::vector<uint8_t> rgb(3 * width * height);
    #pragma omp parallel for if(use_parallel)
    for (int i = 0; i < height; ++i) {
        for (int j = 0; j < width; ++j) {
            int pixel_index = i + j * height;
            int rgb_index = 3 * (i * width + j);
            rgb[rgb_index] = pattern_ptr[pixel_index] >> 16; // R
            rgb[rgb_index + 1] = pattern_ptr[pixel_index] >> 8; // G
            rgb[rgb_index + 2] = pattern_ptr[pixel_index]; // B
        }
    }
    return factory.createArray({ height, width, 3 }, 
                                rgb.begin(), rgb.end(), InputLayout::ROW_MAJOR);
}

// Convert RGB to pattern, assuming column major order in RGB
TypedArray<uint32_t> MexFunction::convertRGB2PatternMex(const TypedArray<uint8_t> rgb, bool use_parallel) {
    const uint8_t *rgb_ptr = getDataPtr<uint8_t>(rgb);
    size_t height = rgb.getDimensions()[0];
    size_t width = rgb.getDimensions()[1];
    std::vector<uint32_t> pattern(width * height);
    #pragma omp parallel for if(use_parallel)
    for (int i = 0; i < height; ++i) {
        for (int j = 0; j < width; ++j) {
            int pixel_index = i + j * height;
            int pattern_index = i * width + j;
            pattern[pattern_index] = (0xFF000000) | (rgb_ptr[pixel_index] << 16) | (rgb_ptr[pixel_index + width * height] << 8) | rgb_ptr[pixel_index + 2 * width * height];
        }
    }
    return factory.createArray({ height, width}, 
                                pattern.begin(), pattern.end(), InputLayout::ROW_MAJOR);
}

// Main function to handle all the MEX function calls
void MexFunction::operator()(ArgumentList outputs, ArgumentList inputs) {
    checkArguments(outputs, inputs);
    if (inputs.size() == 0) {
        // Toggles window state
        if (!isWindowCreated()) {
            open();
        } else {
            close();
        }
        return;
    }

    StringArray func = inputs[0];
    if (inputs.size() == 1) {
        if (func[0] == "open") {
            open();
        } else if (func[0] == "close") {
            close();
        } else if (func[0] == "lock") {
            lock();
        } else if (func[0] == "unlock") {
            unlock();
        } else if (func[0] == "getLockState") {
            outputs[0] = factory.createScalar(LockState);
        } else if (func[0] == "getMexName") {
            outputs[0] = factory.createCharArray(getFunctionName());
        } else if (func[0] == "getBaseDirectory") {
            outputs[0] = getBaseDirectory();
        } else if (func[0] == "isWindowCreated") {
            outputs[0] = factory.createScalar(isWindowCreated());
        } else if (func[0] == "isWindowMinimized") {
            outputs[0] = factory.createScalar(isWindowMinimized());
        } else if (func[0] == "getDisplayModes") {
            outputs[0] = getDisplayModes();
        } else if (func[0] == "getDisplayIndex") {
            outputs[0] = factory.createScalar<double>(DisplayIndex);
        } else if (func[0] == "getWindowHeight") {
            outputs[0] = factory.createScalar(getWindowHeight());
        } else if (func[0] == "getWindowWidth") {
            outputs[0] = factory.createScalar(getWindowWidth());
        } else if (func[0] == "getOperationMode") {
            outputs[0] = getOperationMode();
        } else if (func[0] == "getStaticPatternPath") {
            outputs[0] = getStaticPatternPath();
        } else if (func[0] == "getStaticPattern") {
            outputs[0] = StaticPatternMex;
        } else if (func[0] == "getStaticPatternRGB") {
            outputs[0] = StaticPatternRGBMex;
        } else if (func[0] == "getPatternCanvas") {
            outputs[0] = getPatternCanvas();
        } else if (func[0] == "getPatternCanvasRGB") {
            outputs[0] = getPatternCanvasRGB();
        } else if (func[0] == "getRealCanvas") {
            outputs[0] = getRealCanvas();
        } else if (func[0] == "getRealCanvasRGB") {
            outputs[0] = getRealCanvasRGB();
        } else if (func[0] == "displayColor") {
            displayColor();
        } else if (func[0] == "selectAndProject") {
            selectAndProject();
        } else if (func[0] == "selectAndReadBMP") {
            selectAndReadBMP();
        } else if (func[0] == "getBMPSurfacePattern") {
            outputs[0] = getBMPSurfacePattern();
        } else {
            error("Invalid function name with zero input.");
        }
        return;
    }
    
    if (inputs.size() == 2) {
        if (func[0] == "open") {
            open(inputs[1][0]);
        } else if (func[0] == "setDisplayIndex") {
            setDisplayIndex(inputs[1][0], false);
        } else if (func[0] == "displayColor") {
            TypedArray<double> color = inputs[1];
            displayColor(color[0], color[1], color[2]);
        } else if (func[0] == "setStaticPatternPath") {
            StringArray filename = inputs[1];
            setStaticPatternPath(std::string(filename[0]).c_str());
        } else if (func[0] == "convertPattern2RGB") {
            outputs[0] = convertPattern2RGBMex(inputs[1]);
        } else if (func[0] == "convertRGB2Pattern") {
            outputs[0] = convertRGB2PatternMex(inputs[1]);
        } else {
            error("Invalid function name with one input.");
        }
        return;
    }
    
    if (inputs.size() == 3) {
        if (func[0] == "open") {
            open(inputs[1][0], inputs[2][0]);
        } else if (func[0] == "setStaticPatternPath") {
            StringArray filename = inputs[1];
            setStaticPatternPath(std::string(filename[0]).c_str(), inputs[2][0]);
        } else if (func[0] == "convertPattern2RGB") {
            outputs[0] = convertPattern2RGBMex(inputs[1], inputs[2][0]);
        } else if (func[0] == "convertRGB2Pattern") {
            outputs[0] = convertRGB2PatternMex(inputs[1], inputs[2][0]);
        } else {
            error("Invalid function name with two inputs.");
        }
        return;
    }

    error("Invalid number of inputs: %d.", inputs.size());
}
