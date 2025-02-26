#include <mex.hpp>
#include <mexAdapter.hpp>
#include <PixelCanvas.h>
#include <PatternWindow.h>

using matlab::mex::ArgumentList;
using namespace matlab::data;

// Get a pointor to the raw data of a matlab array
template <typename T>
const T* getDataPtr(Array arr) {
  const TypedArray<T> arr_t = arr;
  TypedIterator<const T> it(arr_t.begin());
  return it.operator->();
}

class CanvasWindow : public BaseWindow, public PixelCanvas {
public:
    CanvasWindow() {
        init(false); // Initialize SDL
    }

    // Every time the window is opened, the canvas is initialized
    void open(std::string arrangement = "Diamond", bool use_parallel = true) {
        BaseWindow::open(false);
        if (Window) {
            initCanvas(WindowHeight, WindowWidth, arrangement);
            updateCanvas2StaticPattern(use_parallel);
        }
    }

    // Every time the window is closed, the canvas is cleared
    void close() {
        BaseWindow::close(false);
        closeCanvas();
    }

    void setStaticPatternPath(const char *filepath, bool verbose = true, bool use_parallel = true) {
        BaseWindow::setStaticPatternPath(filepath, verbose);
        updateCanvas2StaticPattern(use_parallel);
    }

    void projectDynamicPatternFromCanvas(bool verbose) {
        if (PatternCanvas.size() == WindowWidth * WindowHeight) {
            setDynamicPattern((void*) PatternCanvas.data(), verbose);
        }
        else {
            error("Invalid pattern size, number of elements is %d, while window size is (%d, %d).", 
                  PatternCanvas.size(), WindowWidth, WindowHeight);
        }
    }

protected:
    void updateCanvas2StaticPattern(bool use_parallel = true) {
        if (StaticPatternSurface) {
            copyPixel2Pattern((uint32_t*) StaticPatternSurface->pixels, 
                              StaticPatternSurface->w * StaticPatternSurface->h);
            updateReal2Pattern(use_parallel);
        }
    }

    void updateCanvas2DynamicPattern(bool use_parallel = true) {
        if (DynamicPatternSurface) {
            copyPixel2Pattern((uint32_t*) DynamicPatternSurface->pixels, 
                              DynamicPatternSurface->w * DynamicPatternSurface->h);
            updateReal2Pattern(use_parallel);
        }
    }
};


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

    void checkDynamicPatternDimensions(ArgumentList inputs) {
        if (!Window) {
            open();
        }
        size_t nelems = inputs[1].getNumberOfElements();
        if (nelems != WindowWidth * WindowHeight) {
            error("Invalid pattern size, number of elements is %d, while window size is (%d, %d).", nelems, WindowWidth, WindowHeight);
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
            } else if (func[0] == "isWindowCreated") {
                outputs[0] = factory.createScalar(isWindowCreated());
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
            } else if (func[0] == "getLockState") {
                outputs[0] = factory.createScalar(LockState);
            } else if (func[0] == "getMexName") {
                outputs[0] = factory.createCharArray(getFunctionName());
            } else if (func[0] == "getStaticPatternPath") {
                const char *path = getStaticPatternPath();
                if (!path) {
                    outputs[0] = factory.createEmptyArray();
                } else {
                outputs[0] = factory.createCharArray(path);
                }
            } else if (func[0] == "getStaticPattern") {
                SDL_Surface *surface = getStaticPatternSurface();
                if (!surface) {
                    outputs[0] = factory.createEmptyArray();
                } else {
                    outputs[0] = createArrayFromSurface(surface);
                }
            } else if (func[0] == "getDynamicPattern") {
                SDL_Surface *surface = getDynamicPatternSurface();
                if (!surface) {
                    outputs[0] = factory.createEmptyArray();
                } else {
                    outputs[0] = createArrayFromSurface(surface);
                }
            } else if (func[0] == "getPatternCanvas") {
                TypedArray<uint32_t> pattern = factory.createArray(
                    { (uint32_t) NumRows, (uint32_t) NumCols }, 
                    PatternCanvas.begin(), PatternCanvas.end(), InputLayout::ROW_MAJOR);
                outputs[0] = pattern;
            } else if (func[0] == "getPatternCanvasRGB") {
                std::vector<uint8_t> rgb = convertPattern2RGB(PatternCanvas.data(), PatternCanvas.size());
                TypedArray<uint8_t> pattern = factory.createArray(
                    { (uint32_t) NumRows, (uint32_t) NumCols, 3 }, 
                    rgb.begin(), rgb.end(), InputLayout::ROW_MAJOR);
                outputs[0] = pattern;
            } else if (func[0] == "getRealPatternCanvas") {
                TypedArray<uint32_t> pattern = factory.createArray(
                    { (uint32_t) RealNumRows, (uint32_t) RealNumCols }, 
                    RealPatternCanvas.begin(), RealPatternCanvas.end(), InputLayout::ROW_MAJOR);
                    outputs[0] = pattern;
            } else if (func[0] == "getRealPatternCanvasRGB") {
                std::vector<uint8_t> rgb = convertPattern2RGB(RealPatternCanvas.data(), RealPatternCanvas.size());
                TypedArray<uint8_t> pattern = factory.createArray(
                    { (uint32_t) RealNumRows, (uint32_t) RealNumCols, 3 }, 
                    rgb.begin(), rgb.end(), InputLayout::ROW_MAJOR);
                outputs[0] = pattern;
            } else if (func[0] == "getBaseDirectory") {
                outputs[0] = factory.createCharArray(getBaseDirectory());
            } else if (func[0] == "isWindowMinimized") {
                outputs[0] = factory.createScalar(isWindowMinimized());
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
                setDisplayIndex(inputs[1][0]);
            } else if (func[0] == "displayColor") {
                TypedArray<double> color = inputs[1];
                displayColor(color[0], color[1], color[2]);
            } else if (func[0] == "setStaticPatternPath") {
                StringArray filename = inputs[1];
                setStaticPatternPath(std::string(filename[0]).c_str());
            } else if (func[0] == "setDynamicPattern") {
                checkDynamicPatternDimensions(inputs);
                const uint32_t *pattern_ptr = getDataPtr<uint32_t>(inputs[1]);
                setDynamicPattern((void*) pattern_ptr);
            } else {
                error("Invalid function name with one input.");
            }
            return;
        }
        
        if (inputs.size() == 3) {
            if (func[0] == "open") {
                open(inputs[1][0], inputs[2][0]);
            } else if (func[0] == "setDisplayIndex") {
                setDisplayIndex(inputs[1][0], inputs[2][0]);
            } else if (func[0] == "setStaticPatternPath") {
                StringArray filename = inputs[1];
                setStaticPatternPath(std::string(filename[0]).c_str(), inputs[2][0]);
            } else if (func[0] == "setDynamicPattern") {
                checkDynamicPatternDimensions(inputs);
                const uint32_t *pattern_ptr = getDataPtr<uint32_t>(inputs[1]);
                setDynamicPattern((void*) pattern_ptr, inputs[2][0]);
            } else if (func[0] == "convertPattern2RGB") {
                TypedArray<uint32_t> pattern = inputs[1];
                const uint32_t *pattern_ptr = getDataPtr<uint32_t>(pattern);
                size_t num_elements = pattern.getNumberOfElements();
                bool use_parallel = inputs[2][0];
                std::vector<uint8_t> rgb = convertPattern2RGB(pattern_ptr, num_elements, use_parallel);
                TypedArray<uint8_t> rgb_array = factory.createArray(
                    { pattern.getDimensions()[0], pattern.getDimensions()[1], 3 }, 
                    rgb.begin(), rgb.end());
                outputs[0] = rgb_array;
            } else {
                error("Invalid function name with two inputs.");
            }
            return;
        }

        if (inputs.size() == 4) {
            StringArray func = inputs[0];
            if (func[0] == "setStaticPatternPath") {
                StringArray filename = inputs[1];
                setStaticPatternPath(std::string(filename[0]).c_str(), inputs[2][0], inputs[3][0]);
            } else {
                error("Invalid function name with three inputs.");
            }
        }
        else {
            error("Invalid number of inputs.");
        }
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
    TypedArray<uint32_t> createArrayFromSurface(SDL_Surface *surface) {
        uint32_t *pixels = (uint32_t*) surface->pixels;
        std::vector<uint32_t> pixels_vector(pixels, pixels + (surface->w) * (surface->h));
        TypedArray<uint32_t> pattern = factory.createArray(
            { (uint32_t) surface->h, (uint32_t) surface->w }, 
            pixels_vector.begin(), pixels_vector.end(), InputLayout::ROW_MAJOR);
        return pattern;
    }

};
