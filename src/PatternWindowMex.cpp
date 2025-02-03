#include "mex.hpp"
#include "mexAdapter.hpp"
#include "PatternWindow.h"

using matlab::mex::ArgumentList;
using namespace matlab::data;

template <typename T>
const T* getDataPtr(Array arr) {
  const TypedArray<T> arr_t = arr;
  TypedIterator<const T> it(arr_t.begin());
  return it.operator->();
}

class MexFunction : public matlab::mex::Function, public BaseWindow {
private:
    std::shared_ptr<matlab::engine::MATLABEngine> matlab = getEngine();
    ArrayFactory factory;
    bool LockState = false;

public:
    MexFunction() {
        init(false);
    }
    
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

    bool getLockState() {
        return LockState;
    }

    std::u16string getMexName() {
        return getFunctionName();
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
        }
        else if (inputs.size() == 1) {
            StringArray func = inputs[0];
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
                outputs[0] = factory.createScalar(getLockState());
            } else if (func[0] == "getMexName") {
                outputs[0] = factory.createCharArray(getMexName());
            } else if (func[0] == "getStaticPatternPath") {
                const char *path = getStaticPatternPath();
                if (!path) {
                    outputs[0] = factory.createCharArray("");
                } else {
                outputs[0] = factory.createCharArray(getStaticPatternPath());
                }
            } else if (func[0] == "getStaticPattern") {
                SDL_Surface *surface = getStaticPatternSurface();
                if (!surface) {
                    outputs[0] = factory.createEmptyArray();
                } else {
                    uint32_t *pixels = (uint32_t*) surface->pixels;
                    TypedArray<uint32_t> pattern = factory.createArray(
                        { (uint32_t) surface->w, (uint32_t) surface->h }, 
                        pixels, pixels + (surface->w) * (surface->h));
                    outputs[0] = pattern;
                }
            } else if (func[0] == "getDynamicPattern") {
                SDL_Surface *surface = getDynamicPatternSurface();
                if (!surface) {
                    outputs[0] = factory.createEmptyArray();
                } else {
                    uint32_t *pixels = (uint32_t*) surface->pixels;
                    TypedArray<uint32_t> pattern = factory.createArray(
                        { (uint32_t) surface->w, (uint32_t) surface->h }, 
                        pixels, pixels + (surface->w) * (surface->h));
                    outputs[0] = pattern;
                }
            } else if (func[0] == "getBaseDirectory") {
                outputs[0] = factory.createCharArray(getBaseDirectory());
            } else if (func[0] == "isWindowMinimized") {
                outputs[0] = factory.createScalar(isWindowMinimized());
            } else if (func[0] == "displayColor") {
                displayColor();
            } else {
                error("Invalid function name with one input.");
            }
        }
        else if (inputs.size() == 2) {
            StringArray func = inputs[0];
            if (func[0] == "open") {
                open(inputs[1][0]);
            } else if (func[0] == "close") {
                close(inputs[1][0]);
            } else if (func[0] == "setDisplayIndex") {
                setDisplayIndex(inputs[1][0]);
            } else if (func[0] == "displayColor") {
                TypedArray<double> color = inputs[1];
                displayColor(color[0], color[1], color[2]);
            } else if (func[0] == "setStaticPatternPath") {
                StringArray filename = inputs[1];
                setStaticPatternPath(std::string(filename[0]).c_str());
            } else if (func[0] == "setDynamicPattern") {
                const uint32_t *pattern_ptr = getDataPtr<uint32_t>(inputs[1]);
                setDynamicPattern((void*) pattern_ptr);
            } else {
                error("Invalid function name with two inputs.");
            }
        }
        else if (inputs.size() == 3) {
            StringArray func = inputs[0];
            if (func[0] == "setDisplayIndex") {
                setDisplayIndex(inputs[1][0], inputs[2][0]);
            } else if (func[0] == "setStaticPatternPath") {
                StringArray filename = inputs[1];
                setStaticPatternPath(std::string(filename[0]).c_str(), (bool) inputs[2][0]);
            } else if (func[0] == "setDynamicPattern") {
                const uint32_t *pattern_ptr = getDataPtr<uint32_t>(inputs[1]);
                setDynamicPattern((void*) pattern_ptr, inputs[2][0]);
            } else {
                error("Invalid function name with three inputs.");
            }
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
};
