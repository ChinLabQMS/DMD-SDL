// To compile: mex src/PatternWindowMex.cpp src/PatternWindow.cpp -R2018a -DTEST=0 -Iinclude -Llib -lSDL3 -lkernel32 -luser32 -lgdi32 -lwinmm -limm32 -lole32 -loleaut32 -lversion -luuid -ladvapi32 -lsetupapi -lshell32 -ldinput8

#include "mex.hpp"
#include "mexAdapter.hpp"
#include "PatternWindow.h"

using matlab::mex::ArgumentList;
using namespace matlab::data;

class MexFunction : public matlab::mex::Function, public PatternWindow {
    std::shared_ptr<matlab::engine::MATLABEngine> matlab = getEngine();
    ArrayFactory factory;
    std::ostringstream stream;

public:
    MexFunction() {
        init();
    }

    ~MexFunction() {
    }

    void printf(const char* format, ...) {
        char buffer[256];
        va_list args;
        va_start(args, format);
        vsprintf(buffer, format, args);
        stream << buffer << std::endl;
        matlab->feval(u"fprintf", 0, std::vector<Array>({ factory.createScalar(stream.str()) }));
        stream.str("");
        va_end(args);
    }

    void operator()(ArgumentList outputs, ArgumentList inputs) {
        checkArguments(outputs, inputs);
        if (inputs.size() == 0) {
            if (!(isWindowCreated())) {
                open();
            }
            else
                close();
            return;
        }
        if (inputs.size() == 1) {
            
        }
        if (inputs.size() == 2) {

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
