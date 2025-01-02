#include "mex.hpp"
#include "mexAdapter.hpp"
#include "PatternWindow.h"

using matlab::mex::ArgumentList;
using namespace matlab::data;

class MexFunction : public matlab::mex::Function, public BaseWindow {
    std::shared_ptr<matlab::engine::MATLABEngine> matlab = getEngine();
    ArrayFactory factory;

public:
    MexFunction() {
        init();
    }

    ~MexFunction() {
        printf("Quitting SDL...");
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

    void error(const char* message) {
        matlab->feval(u"error", 0, std::vector<Array>({ factory.createScalar(message) }));
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

    void operator()(ArgumentList outputs, ArgumentList inputs) {
        checkArguments(outputs, inputs);
        if (inputs.size() == 0) {
            // Toggles window state
            if (!(isWindowCreated())) {
                open();
            }
            else
                close();
            return;
        }
        if (inputs.size() == 1) {
            StringArray func = inputs[0];
            if (func[0] == "open")
                open();
            else if (func[0] == "close")
                close();
            else if (func[0] == "isWindowCreated")
                outputs[0] = factory.createScalar(isWindowCreated());
            else if (func[0] == "getDisplayModes")
                outputs[0] = getDisplayModes();
            else if (func[0] == "getDisplayIndex")
                outputs[0] = factory.createScalar(getDisplayIndex());
            else
                error("Invalid function name with one input.");
        }
        if (inputs.size() == 2) {
            StringArray func = inputs[0];
            if (func[0] == "open")
                open(inputs[1][0]);
            else if (func[0] == "setDisplayIndex")
                setDisplayIndex(inputs[1][0]);
            else if (func[0] == "displayColor") {
                TypedArray<double> color = inputs[1];
                displayColor(color[0], color[1], color[2]);
            }
            else if (func[0] == "projectFromFile") {
                StringArray filename = inputs[1];
                projectFromFile(std::string(filename[0]).c_str());
            }
            else {
                error("Invalid function name with two inputs.");
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
            matlab->feval(u"error", 0, std::vector<Array>({ factory.createScalar("First input must be scalar string") }));
        }
    }
};
