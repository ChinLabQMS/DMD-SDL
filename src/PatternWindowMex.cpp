#include <mex.hpp>
#include <mexAdapter.hpp>

using matlab::mex::ArgumentList;
using namespace matlab::data;

// Get a pointor to the raw data of a matlab array
template <typename T>
const T* getDataPtr(Array arr) {
  const TypedArray<T> arr_t = arr;
  TypedIterator<const T> it(arr_t.begin());
  return it.operator->();
}

class MexFunction : public matlab::mex::Function, public CanvasWindow {
private:
    std::shared_ptr<matlab::engine::MATLABEngine> matlab = getEngine();
    ArrayFactory factory;
    bool LockState = false;

public:
    // Overloaded printf function to display messages in MATLAB console
    void printf(const char* format, ...) {
        char buffer[256];
        va_list args;
        va_start(args, format);
        vsprintf(buffer, format, args);
        va_end(args);
        matlab->feval(u"fprintf", 0, std::vector<Array>({ factory.createScalar(buffer) }));
        matlab->feval(u"fprintf", 0, std::vector<Array>({ factory.createScalar("\n") }));
    }

    // Overloaded printf function to display messages in MATLAB console
    void error(const char* format, ...) {
        char buffer[256];
        va_list args;
        va_start(args, format);
        vsprintf(buffer, format, args);
        va_end(args);
        matlab->feval(u"error", 0, std::vector<Array>({ factory.createScalar(buffer) }));
    }

    StructArray getDisplayModes() {
        int num_displays;
        SDL_DisplayID *displays = SDL_GetDisplays(&num_displays);
        StructArray display_modes = factory.createStructArray({ (unsigned int) num_displays }, 
                                { "Width", "Height", "RefreshRate" });
        for (int i = 0; i < num_displays; i++) {
            const SDL_DisplayMode *mode = SDL_GetDesktopDisplayMode(displays[i]);
            display_modes[i]["Width"] = factory.createScalar(mode->w);
            display_modes[i]["Height"] = factory.createScalar(mode->h);
            display_modes[i]["RefreshRate"] = factory.createScalar(mode->refresh_rate);
        }
        return display_modes;
    }

    void lock() {
        if (!LockState) {
            mexLock();
            LockState = true;
        }
    }

    void unlock() {
        if (LockState) {
            mexUnlock();
            LockState = false;
        }
    }

    void operator()(ArgumentList outputs, ArgumentList inputs) {
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
                outputs[0] = factory.createCharArray(getBaseDirectory());
            } else if (func[0] == "isWindowCreated") {
                outputs[0] = factory.createScalar(isWindowCreated());
            } else if (func[0] == "isWindowMinimized") {
                outputs[0] = factory.createScalar(isWindowMinimized());
            } else if (func[0] == "getDisplayModes") {
                outputs[0] = getDisplayModes();
            } else if (func[0] == "getDisplayIndex") {
                outputs[0] = factory.createScalar(getDisplayIndex());
            } else if (func[0] == "getWindowHeight") {
                outputs[0] = factory.createScalar(getWindowHeight());
            } else if (func[0] == "getWindowWidth") {
                outputs[0] = factory.createScalar(getWindowWidth());
            } else if (func[0] == "getStaticMode") {
                outputs[0] = factory.createScalar(getStaticMode());
            } else if (func[0] == "getStaticPatternPath") {
                if (StaticPatternPath)
                    outputs[0] = factory.createCharArray(StaticPatternPath);
                else
                    outputs[0] = factory.createEmptyArray();
            } else if (func[0] == "getStaticPattern") {
                if (StaticPatternSurface)
                    outputs[0] = createArrayFromSurface(StaticPatternSurface);
                else
                    outputs[0] = factory.createEmptyArray();
            } else if (func[0] == "getStaticPatternRGB") {
                if (StaticPatternSurface)
                    outputs[0] = createRGBArrayFromSurface(StaticPatternSurface);
                else
                    outputs[0] = factory.createEmptyArray();
            } else if (func[0] == "getDynamicPattern") {
                if (DynamicPatternSurface)
                    outputs[0] = createArrayFromSurface(DynamicPatternSurface);
                else
                    outputs[0] = factory.createEmptyArray();
            } else if (func[0] == "getDynamicPatternRGB") {
                if (DynamicPatternSurface)
                    outputs[0] = createRGBArrayFromSurface(DynamicPatternSurface);
                else
                    outputs[0] = factory.createEmptyArray();
            } else if (func[0] == "getPatternCanvas") {
                if (PatternCanvas.size() > 0)
                    outputs[0] = createArrayFromVector(PatternCanvas, WindowHeight, WindowWidth);
                else
                    outputs[0] = factory.createEmptyArray();
            } else if (func[0] == "getPatternCanvasRGB") {
                if (PatternCanvas.size() > 0)
                    outputs[0] = createRGBArrayFromVector(PatternCanvas, WindowHeight, WindowWidth);
                else
                    outputs[0] = factory.createEmptyArray();
            } else if (func[0] == "getRealPatternCanvas") {
                if (RealPatternCanvas.size() > 0)
                    outputs[0] = createArrayFromVector(RealPatternCanvas, RealNumRows, RealNumCols);
                else
                    outputs[0] = factory.createEmptyArray();
            } else if (func[0] == "getRealPatternCanvasRGB") {
                if (RealPatternCanvas.size() > 0)
                    outputs[0] = createRGBArrayFromVector(RealPatternCanvas, RealNumRows, RealNumCols);
                else
                    outputs[0] = factory.createEmptyArray();
            } else if (func[0] == "displayColor") {
                displayColor();
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
            } else if (func[0] == "setDynamicPattern") {
                checkDynamicPatternDimensions(inputs[1]);
                const uint32_t *pattern_ptr = getDataPtr<uint32_t>(inputs[1]);
                setDynamicPattern((void*) pattern_ptr);
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
            } else if (func[0] == "setDynamicPattern") {
                checkDynamicPatternDimensions(inputs[1]);
                const uint32_t *pattern_ptr = getDataPtr<uint32_t>(inputs[1]);
                setDynamicPattern((void*) pattern_ptr, inputs[2][0]);
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
            if (func[0] == "convertRGB2Pattern") {
                outputs[0] = convertRGB2PatternMex(inputs[1], inputs[2][0], inputs[3][0]);
            } else {
                error("Invalid function name with three inputs.");
            }
            return;
        }

        error("Invalid number of inputs: %d.", inputs.size());
    }

    void checkArguments(ArgumentList outputs, ArgumentList inputs) {
        // Check number of arguments: if empty, assume init function
        if (inputs.size() == 0) {
            return;
        }
        // Check first argument: First input must be scalar string that specifies the function to call
        if (inputs[0].getType() != ArrayType::MATLAB_STRING || inputs[0].getNumberOfElements() != 1){
            error("First input must be a scalar string.");
        }
    }

protected:
    void checkDynamicPatternDimensions(const TypedArray<uint32_t> pattern) {
        if (!Window) {
            open();
        }
        size_t nelems = pattern.getNumberOfElements();
        if (nelems != WindowWidth * WindowHeight) {
            error("Invalid pattern size, number of elements is %d, while window size is (%d, %d).", nelems, WindowWidth, WindowHeight);
        }
    }

    TypedArray<uint32_t> createArrayFromSurface(const SDL_Surface *surface) {
        uint32_t *pixels = (uint32_t*) surface->pixels;
        std::vector<uint32_t> pixels_vector(pixels, pixels + (surface->w) * (surface->h));
        TypedArray<uint32_t> pattern = factory.createArray(
            { (uint32_t) surface->h, (uint32_t) surface->w }, 
            pixels_vector.begin(), pixels_vector.end(), InputLayout::ROW_MAJOR);
        return pattern;
    }

    TypedArray<uint8_t> createRGBArrayFromSurface(const SDL_Surface *surface, bool use_parallel = true) {
        TypedArray<uint32_t> pattern = createArrayFromSurface(surface);
        return convertPattern2RGBMex(pattern, use_parallel);
    }

    TypedArray<uint32_t> createArrayFromVector(const std::vector<uint32_t> vec, size_t nrows, size_t ncols) {
        TypedArray<uint32_t> pattern = factory.createArray(
            { (uint32_t) nrows, (uint32_t) ncols }, 
            vec.begin(), vec.end(), InputLayout::ROW_MAJOR);
        return pattern;
    }

    TypedArray<uint8_t> createRGBArrayFromVector(const std::vector<uint32_t> vec, size_t nrows, size_t ncols, bool use_parallel = true) {
        TypedArray<uint32_t> pattern = createArrayFromVector(vec, nrows, ncols);
        return convertPattern2RGBMex(pattern, use_parallel);
    }

    TypedArray<uint8_t> convertPattern2RGBMex(const TypedArray<uint32_t> pattern, bool use_parallel = true) {
        const uint32_t *pattern_ptr = getDataPtr<uint32_t>(pattern);
        std::vector<uint8_t> rgb = convertPattern2RGB(pattern_ptr, pattern.getNumberOfElements(), use_parallel);
        TypedArray<uint8_t> rgb_array = factory.createArray(
            { pattern.getDimensions()[0], pattern.getDimensions()[1], 3 }, 
            rgb.begin(), rgb.end());
        return rgb_array;
    }

    TypedArray<uint32_t> convertRGB2PatternMex(const TypedArray<uint8_t> rgb, bool alpha_mask = true, bool use_parallel = true) {
        if (rgb.getDimensions()[2] != 3) {
            error("Invalid number of color channels, must be 3.");
        }
        const uint8_t *rgb_ptr = getDataPtr<uint8_t>(rgb);
        std::vector<uint32_t> pattern = convertRGB2Pattern(rgb_ptr, rgb.getNumberOfElements(), use_parallel);
        TypedArray<uint32_t> pattern_array = factory.createArray(
            { rgb.getDimensions()[0], rgb.getDimensions()[1] }, 
            pattern.begin(), pattern.end());
        return pattern_array;
    }
};

int main() {
    MexFunction mexFunction;
    
    return 0;
}
