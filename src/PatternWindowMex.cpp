#include "PatternWindowMex.h"

MexFunction::MexFunction() {
    init(false); // Initialize SDL upon creation
}

// Override printf function to display messages in MATLAB console
void MexFunction::printf(const char* format, ...) {
    char buffer[256];
    va_list args;
    va_start(args, format);
    vsprintf(buffer, format, args);
    va_end(args);
    matlab->feval(u"fprintf", 0, std::vector<Array>({ factory.createScalar(buffer) }));
    matlab->feval(u"fprintf", 0, std::vector<Array>({ factory.createScalar("\n") }));
}

// Override warn function to display messages in MATLAB console
void MexFunction::warn(const char* format, ...) {
    char buffer[256];
    va_list args;
    va_start(args, format);
    vsprintf(buffer, format, args);
    va_end(args);
    matlab->feval(u"warning", 0, std::vector<Array>({ factory.createScalar(buffer) }));
}

// Override printf function to display messages in MATLAB console
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

void MexFunction::updateStaticPatternProperties(bool use_parallel) {
    PatternWindow::updateStaticPatternProperties(use_parallel);
    // Update the Matlab array of the static pattern
    if (StaticPatternSurface) {
        uint32_t * pixels = (uint32_t*) StaticPatternSurface->pixels;
        StaticPatternMex = factory.createArray({ 
            (size_t) StaticPatternSurface->h, (size_t) StaticPatternSurface->w }, 
            pixels, 
            pixels + StaticPatternSurface->h * StaticPatternSurface->w, 
            InputLayout::ROW_MAJOR);
        StaticPatternRGBMex = factory.createArray(
            { (size_t) WindowHeight, (size_t) WindowWidth, 3 }, 
            StaticPatternRGB.begin(), StaticPatternRGB.end(), 
            InputLayout::ROW_MAJOR);
        StaticPatternRealMex = factory.createArray(
            { (size_t) RealNumRows, (size_t) RealNumCols }, 
            StaticPatternReal.begin(), 
            StaticPatternReal.end(),
            InputLayout::ROW_MAJOR);
        StaticPatternRealRGBMex = factory.createArray(
            { (size_t) RealNumRows, (size_t) RealNumCols, 3 }, 
            StaticPatternRealRGB.begin(), StaticPatternRealRGB.end(), 
            InputLayout::ROW_MAJOR);
    }
    else {
        StaticPatternMex = factory.createArray({0, 0}, (uint32_t *) nullptr, (uint32_t *) nullptr);
        StaticPatternRGBMex = factory.createArray({0, 0, 3}, (uint8_t *) nullptr, (uint8_t *) nullptr);
        StaticPatternRealMex = factory.createArray({0, 0}, (uint32_t *) nullptr, (uint32_t *) nullptr);
        StaticPatternRealRGBMex = factory.createArray({0, 0, 3}, (uint8_t *) nullptr, (uint8_t *) nullptr);
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

TypedArray<double> MexFunction::getDynamicMemorySize() {
    return factory.createScalar((double) DynamicMemory.size());
}

TypedArray<double> MexFunction::getPatternMemorySize() {
    return factory.createScalar((double) PatternMemory.size());
}

TypedArray<uint32_t> MexFunction::getPatternMemory(int index) {
    if  ((index >= 0) && (index < PatternMemory.size())) {
        return factory.createArray({ (size_t) PatternMemory[index].size() }, 
                                    PatternMemory[index].begin(), PatternMemory[index].end(), 
                                    InputLayout::ROW_MAJOR);
    } else {
        return factory.createArray({0}, (uint32_t *) nullptr, (uint32_t *) nullptr);
    }
}

TypedArray<uint32_t> MexFunction::getPatternMemoryReal(int index, uint32_t background_color, bool use_parallel) {
    if  ((index >= 0) && (index < PatternMemory.size())) {
        std::vector<uint32_t> pattern_real = PixelCanvas::getPatternMemoryReal(index, background_color, use_parallel);
        return factory.createArray({ (size_t) RealNumRows, (size_t) RealNumCols }, 
                                    pattern_real.begin(), pattern_real.end(), 
                                    InputLayout::ROW_MAJOR);
    } else {
        return factory.createArray({0, 0}, (uint32_t *) nullptr, (uint32_t *) nullptr);
    }
}

TypedArray<uint8_t> MexFunction::getPatternMemoryRGB(int index, bool use_parallel) {
    if  ((index >= 0) && (index < PatternMemory.size())) {
        std::vector<uint8_t> pattern_rgb = PixelCanvas::getPatternMemoryRGB(index, use_parallel);
        return factory.createArray({ (size_t) NumRows, (size_t) NumCols, 3 }, 
                                    pattern_rgb.begin(), pattern_rgb.end(), 
                                    InputLayout::ROW_MAJOR);
    } else {
        return factory.createArray({0, 0, 3}, (uint8_t *) nullptr, (uint8_t *) nullptr);
    }
}

TypedArray<uint8_t> MexFunction::getPatternMemoryRealRGB(int index, uint32_t background_color, bool use_parallel) {
    if  ((index >= 0) && (index < PatternMemory.size())) {
        std::vector<uint8_t> pattern_real_rgb = PixelCanvas::getPatternMemoryRealRGB(index, background_color, use_parallel);
        return factory.createArray({ (size_t) RealNumRows, (size_t) RealNumCols, 3 }, 
                                    pattern_real_rgb.begin(), pattern_real_rgb.end(), 
                                    InputLayout::ROW_MAJOR);
    } else {
        return factory.createArray({0, 0, 3}, (uint8_t *) nullptr, (uint8_t *) nullptr);
    }
}

// Get the pattern canvas as a MATLAB array
TypedArray<uint32_t> MexFunction::getDynamicMemory(int index) {
    if  ((index >= 0) && (index < DynamicMemory.size())) {
        return factory.createArray({ (size_t) NumRows, (size_t) NumCols }, 
                                    DynamicMemory[index].begin(), DynamicMemory[index].end(), 
                                    InputLayout::ROW_MAJOR);
    } else {
        return factory.createArray({0, 0}, (uint32_t *) nullptr, (uint32_t *) nullptr);
    }
}

// Get the real canvas as a MATLAB array
TypedArray<uint32_t> MexFunction::getDynamicMemoryReal(int index, uint32_t background_color, bool use_parallel) {
    if  ((index >= 0) && (index < DynamicMemory.size())) {
        std::vector<uint32_t> pattern_real = PixelCanvas::getDynamicMemoryReal(index, background_color, use_parallel);
        return factory.createArray({ (size_t) RealNumRows, (size_t) RealNumCols }, 
                                    pattern_real.begin(), pattern_real.end(), 
                                    InputLayout::ROW_MAJOR);
    } else {
        return factory.createArray({0, 0}, (uint32_t *) nullptr, (uint32_t *) nullptr);
    }
}

// Get the pattern canvas as a MATLAB RGB array
TypedArray<uint8_t> MexFunction::getDynamicMemoryRGB(int index, bool use_parallel) {
    if  ((index >= 0) && (index < DynamicMemory.size())) {
        std::vector<uint8_t> pattern_rgb = PixelCanvas::getDynamicMemoryRGB(index, use_parallel);
        return factory.createArray({ (size_t) NumRows, (size_t) NumCols, 3 },
                                    pattern_rgb.begin(), pattern_rgb.end(), 
                                    InputLayout::ROW_MAJOR);
    } else {
        return factory.createArray({0, 0, 3}, (uint8_t *) nullptr, (uint8_t *) nullptr);
    }
}

// Get the real canvas as a MATLAB RGB array
TypedArray<uint8_t> MexFunction::getDynamicMemoryRealRGB(int index, uint32_t background_color, bool use_parallel) {
    if  ((index >= 0) && (index < DynamicMemory.size())) {
        std::vector<uint8_t> pattern_real_rgb = PixelCanvas::getDynamicMemoryRealRGB(index, background_color, use_parallel);
        return factory.createArray({ (size_t) RealNumRows, (size_t) RealNumCols, 3 }, 
                                    pattern_real_rgb.begin(), pattern_real_rgb.end(), 
                                    InputLayout::ROW_MAJOR);
    } else {
        return factory.createArray({0, 0, 3}, (uint8_t *) nullptr, (uint8_t *) nullptr);
    }
}

TypedArray<bool> MexFunction::displayPatternMemory(const TypedArray<double> indices, uint32_t delay, bool verbose, bool use_parallel) {
    std::vector<uint32_t> index_vector;
    for (size_t i = 0; i < indices.getNumberOfElements(); ++i) {
        index_vector.push_back(static_cast<uint32_t>(indices[i]));
    }
    return factory.createScalar(PatternWindow::displayPatternMemory(index_vector.size(), index_vector.data(), delay, verbose, use_parallel));
}

TypedArray<bool> MexFunction::displayDynamicMemory(const TypedArray<double> indices, uint32_t delay, bool verbose, bool use_parallel) {
    std::vector<uint32_t> index_vector;
    for (size_t i = 0; i < indices.getNumberOfElements(); ++i) {
        index_vector.push_back(static_cast<uint32_t>(indices[i]));
    }
    return factory.createScalar(PatternWindow::displayDynamicMemory(index_vector.size(), index_vector.data(), delay, verbose, use_parallel));
}
void MexFunction::generateBlackTweezerPattern(TypedArray<double> coords, 
                                     TypedArray<double> radius,
                                     TypedArray<double> shift,
                                     int num_RGB_buffers,
                                     int num_bin_frames, 
                                     bool use_parallel) {
    if (coords.getDimensions()[1]!= 2) {
        error("Second input must be a 2D array with two columns.");
    }
    int num_tweezers = coords.getDimensions()[0];
    std::vector<double> x0(num_tweezers);
    std::vector<double> y0(num_tweezers);
    for (int i = 0; i < num_tweezers; i++) {
        x0[i] = coords[i][0];
        y0[i] = coords[i][1];
    }
    PatternWindow::generateBlackTweezerPattern(num_tweezers, x0.data(), y0.data(), 
        radius[0], shift[0], shift[1], num_RGB_buffers, num_bin_frames, use_parallel);
}

void MexFunction::generateBlackTweezerPatternStatic(TypedArray<double> V, 
                                                    TypedArray<double> R, 
                                                    TypedArray<double> sites, 
                                                    double radius) {
    std::vector<double> calib(6);
    calib[0] = V[0][0];
    calib[1] = V[0][1];
    calib[2] = V[1][0];
    calib[3] = V[1][1];
    calib[4] = R[0];
    calib[5] = R[1];
    std::vector<double> x0(sites.getDimensions()[0]);
    std::vector<double> y0(sites.getDimensions()[0]);
    for (int i = 0; i < sites.getDimensions()[0]; ++i) {
        x0[i] = sites[i][0];
        y0[i] = sites[i][1];
    }
    PatternWindow::generateBlackTweezerPatternStatic(calib, x0, y0, radius);
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
            open2("Diamond", true, true);
        } else {
            close(true);
        }
        return;
    }

    StringArray func = inputs[0];
    if (inputs.size() == 1) {
        if (func[0] == "open") {
            open2("Diamond", true, true);
        } else if (func[0] == "close") {
            close(true);
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
            outputs[0] = factory.createScalar(WindowHeight);
        } else if (func[0] == "getWindowWidth") {
            outputs[0] = factory.createScalar(WindowWidth);
        } else if (func[0] == "getOperationMode") {
            outputs[0] = getOperationMode();
        } else if (func[0] == "getStaticPatternPath") {
            outputs[0] = getStaticPatternPath();
        } else if (func[0] == "getStaticPattern") {
            outputs[0] = StaticPatternMex;
        } else if (func[0] == "getStaticPatternRGB") {
            outputs[0] = StaticPatternRGBMex;
        } else if (func[0] == "getStaticPatternReal") {
            outputs[0] = StaticPatternRealMex;
        } else if (func[0] == "getStaticPatternRealRGB") {
            outputs[0] = StaticPatternRealRGBMex;
        } else if (func[0] == "getDynamicMemorySize") {
            outputs[0] = getDynamicMemorySize(); 
        } else if (func[0] == "getPatternMemorySize") {
            outputs[0] = getPatternMemorySize(); 
        } else if (func[0] == "displayColor") {
            displayColor(0, 0, 0, true);
        } else if (func[0] == "selectAndProject") {
            selectAndProject(NULL, true);
        } else if (func[0] == "selectAndLoadPatternMemory") {
            selectAndLoadPatternMemory(NULL, true, true);
        } else if (func[0] == "clearPatternMemory") {
            clearPatternMemoryAll();
        } else if (func[0] == "clearDynamicMemory") {
            clearDynamicMemoryAll();
        } else if (func[0] == "displayDynamicMemoryAll") {
            displayDynamicMemoryAll(0, true, true);
        } else {
            error("Invalid function name with zero input.");
        }
        return;
    }
    
    if (inputs.size() == 2) {
        if (func[0] == "open") {
            open2(inputs[1][0], true, true);
        } else if (func[0] == "close") {
            close(inputs[1][0]);
        } else if (func[0] == "getPatternMemory") {
            outputs[0] = getPatternMemory(inputs[1][0]);
        } else if (func[0] == "getPatternMemoryRGB") {
            outputs[0] = getPatternMemoryRGB(inputs[1][0], true);
        } else if (func[0] == "getPatternMemoryReal") {
            outputs[0] = getPatternMemoryReal(inputs[1][0], 0xFFFF0000, true);
        } else if (func[0] == "getPatternMemoryRealRGB") {
            outputs[0] = getPatternMemoryRealRGB(inputs[1][0], 0xFFFF0000, true);
        } else if (func[0] == "getDynamicMemory") {
            outputs[0] = getDynamicMemory(inputs[1][0]);
        } else if (func[0] == "getDynamicMemoryRGB") {
            outputs[0] = getDynamicMemoryRGB(inputs[1][0], true);
        } else if (func[0] == "getDynamicMemoryReal") {
            outputs[0] = getDynamicMemoryReal(inputs[1][0], 0xFFFF0000, true);
        } else if (func[0] == "getDynamicMemoryRealRGB") {
            outputs[0] = getDynamicMemoryRealRGB(inputs[1][0], 0xFFFF0000, true);
        } else if (func[0] == "setDisplayIndex") {
            setDisplayIndex(inputs[1][0], true);
        } else if (func[0] == "displayColor") {
            TypedArray<double> color = inputs[1];
            displayColor(color[0], color[1], color[2], true);
        } else if (func[0] == "setStaticPatternPath") {
            StringArray filename = inputs[1];
            setStaticPatternPath2(std::string(filename[0]).c_str(), true, true);
        } else if (func[0] == "loadPatternMemoryFromPath") {
            StringArray filename = inputs[1];
            loadPatternMemoryFromFile(std::string(filename[0]).c_str(), true, true);
        } else if (func[0] == "selectAndProject") {
            selectAndProject(NULL, inputs[1][0]);
        } else if (func[0] == "selectAndLoadPatternMemory") {
            selectAndLoadPatternMemory(NULL, inputs[1][0], true);
        } else if (func[0] == "displayPatternMemory") {
            outputs[0] = displayPatternMemory(inputs[1], 0, true, true);
        } else if (func[0] == "displayDynamicMemory") {
            outputs[0] = displayDynamicMemory(inputs[1], 0, true, true);
        } else if (func[0] == "displayDynamicMemoryAll") {
            displayDynamicMemoryAll(inputs[1][0], true, true);
        } else if (func[0] == "convertPattern2RGB") {
            outputs[0] = convertPattern2RGBMex(inputs[1], true);
        } else if (func[0] == "convertRGB2Pattern") {
            outputs[0] = convertRGB2PatternMex(inputs[1], true);
        } else {
            error("Invalid function name with one input.");
        }
        return;
    }
    
    if (inputs.size() == 3) {
        if (func[0] == "open") {
            open2(inputs[1][0], inputs[2][0], true);
        } else if (func[0] == "getPatternMemoryRGB") {
            outputs[0] = getPatternMemoryRGB(inputs[1][0], inputs[2][0]);
        } else if (func[0] == "getPatternMemoryReal") {
            outputs[0] = getPatternMemoryReal(inputs[1][0], inputs[2][0], true);
        } else if (func[0] == "getPatternMemoryRealRGB") {
            outputs[0] = getPatternMemoryRealRGB(inputs[1][0], inputs[2][0], true);
        } else if (func[0] == "getDynamicMemoryRGB") {
            outputs[0] = getDynamicMemoryRGB(inputs[1][0], inputs[2][0]);
        } else if (func[0] == "getDynamicMemoryReal") {
            outputs[0] = getDynamicMemoryReal(inputs[1][0], inputs[2][0], true);
        } else if (func[0] == "getDynamicMemoryRealRGB") {
            outputs[0] = getDynamicMemoryRealRGB(inputs[1][0], inputs[2][0], true);
        } else if (func[0] == "setDisplayIndex") {
            setDisplayIndex(inputs[1][0], inputs[2][0]);
        } else if (func[0] == "displayColor") {
            displayColor(inputs[1][0], inputs[1][1], inputs[1][2], inputs[2][0]);
        } else if (func[0] == "setStaticPatternPath") {
            StringArray filename = inputs[1];
            setStaticPatternPath2(std::string(filename[0]).c_str(), inputs[2][0], true);
        } else if (func[0] == "loadPatternMemoryFromPath") {
            StringArray filename = inputs[1];
            loadPatternMemoryFromFile(std::string(filename[0]).c_str(), inputs[2][0], true);
        } else if (func[0] == "selectAndLoadPatternMemory") {
            selectAndLoadPatternMemory(NULL, inputs[1][0], inputs[2][0]);
        } else if (func[0] == "displayPatternMemory") {
            outputs[0] = displayPatternMemory(inputs[1], inputs[2][0], true, true);
        } else if (func[0] == "displayDynamicMemory") {
            outputs[0] = displayDynamicMemory(inputs[1], inputs[2][0], true, true);
        } else if (func[0] == "displayDynamicMemoryAll") {
            displayDynamicMemoryAll(inputs[1][0], inputs[2][0], true);
        } else if (func[0] == "convertPattern2RGB") {
            outputs[0] = convertPattern2RGBMex(inputs[1], inputs[2][0]);
        } else if (func[0] == "convertRGB2Pattern") {
            outputs[0] = convertRGB2PatternMex(inputs[1], inputs[2][0]);
        } else {
            error("Invalid function name with two inputs.");
        }
        return;
    }

    if (inputs.size() == 4) {
        if (func[0] == "open") {
            open2(inputs[1][0], inputs[2][0], inputs[3][0]);
        } else if (func[0] == "getPatternMemoryReal") {
            outputs[0] = getPatternMemoryReal(inputs[1][0], inputs[2][0], inputs[3][0]);
        } else if (func[0] == "getPatternMemoryRealRGB") {
            outputs[0] = getPatternMemoryRealRGB(inputs[1][0], inputs[2][0], inputs[3][0]);
        } else if (func[0] == "setStaticPatternPath") {
            StringArray filename = inputs[1];
            setStaticPatternPath2(std::string(filename[0]).c_str(), inputs[2][0], inputs[3][0]);
        } else if (func[0] == "loadPatternMemoryFromPath") {
            StringArray filename = inputs[1];
            loadPatternMemoryFromFile(std::string(filename[0]).c_str(), inputs[2][0], inputs[3][0]);
        } else if (func[0] == "displayPatternMemory") {
            outputs[0] = displayPatternMemory(inputs[1], inputs[2][0], inputs[3][0], true);
        } else if (func[0] == "displayDynamicMemory") {
            outputs[0] = displayDynamicMemory(inputs[1], inputs[2][0], inputs[3][0], true);
        } else if (func[0] == "displayDynamicMemoryAll") {
            displayDynamicMemoryAll(inputs[1][0], inputs[2][0], inputs[3][0]);
        } else {
            error("Invalid function name with three inputs.");
        }
        return;
    }

    if (inputs.size() == 5) {
        if (func[0] == "displayPatternMemory") {
            outputs[0] = displayPatternMemory(inputs[1], inputs[2][0], inputs[3][0], inputs[4][0]);
        } else if (func[0] == "displayDynamicMemory") {
            outputs[0] = displayDynamicMemory(inputs[1], inputs[2][0], inputs[3][0], inputs[4][0]);
        } else if (func[0] == "generateBlackTweezerPatternStatic") {
            generateBlackTweezerPatternStatic(inputs[1], inputs[2], inputs[3], inputs[4][0]);
        } else {
            error("Invalid function name with four inputs.");
        }
        return;
    }

    if (inputs.size() == 6) {
        if (func[0] == "generateBlackTweezerPattern") {
            generateBlackTweezerPattern(inputs[1], inputs[2], inputs[3], inputs[4][0], inputs[5][0], true);
        } else {
            error("Invalid function name with five inputs.");
        }
        return;
    }

    if (inputs.size() == 7) {
        if (func[0] == "generateBlackTweezerPattern") {
            generateBlackTweezerPattern(inputs[1], inputs[2], inputs[3], inputs[4][0], inputs[5][0], inputs[6][0]);
        } else {
            error("Invalid function name with six inputs.");
        }
        return;
    }

    error("Invalid number of inputs: %d.", inputs.size());
}
