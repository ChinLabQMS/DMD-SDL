#ifndef PATTERNWINDOWMEX_H
#define PATTERNWINDOWMEX_H

#include "PatternWindow.h"
#include <variant>
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

class MexFunction : public matlab::mex::Function, public PatternWindow {
private:
    std::shared_ptr<matlab::engine::MATLABEngine> matlab = getEngine();
    ArrayFactory factory;
    bool LockState = false;
    TypedArray<uint8_t> StaticPatternRGBMex = factory.createArray({0, 0, 3}, 
                                              (uint8_t *) nullptr, (uint8_t *) nullptr);
    TypedArray<uint32_t> StaticPatternMex = factory.createArray({0, 0}, 
                                              (uint32_t *) nullptr, (uint32_t *) nullptr);
    void checkArguments(ArgumentList outputs, ArgumentList inputs);
public:
    MexFunction();
    void printf(const char* format, ...) override;
    void warn(const char* format, ...) override;
    void error(const char* format, ...) override;
    void lock();
    void unlock();
    void setStaticPatternPath(const char* filepath, bool verbose) override;
    void setStaticPatternPath(const char* filepath, bool verbose, bool use_parallel) override;
    CharArray getOperationMode();
    CharArray getBaseDirectory();
    CharArray getStaticPatternPath();
    StructArray getDisplayModes();
    TypedArray<uint32_t> getBMPSurfacePattern();
    TypedArray<uint32_t> getPatternCanvas();
    TypedArray<uint8_t> getPatternCanvasRGB(bool use_parallel = true);
    TypedArray<uint32_t> getRealCanvas();
    TypedArray<uint8_t> getRealCanvasRGB(bool use_parallel = true);
    TypedArray<uint8_t> convertPattern2RGBMex(const TypedArray<uint32_t> pattern, bool use_parallel = true);
    TypedArray<uint32_t> convertRGB2PatternMex(const TypedArray<uint8_t> rgb, bool use_parallel = true);
    void operator()(ArgumentList outputs, ArgumentList inputs) override;
};

#endif // PATTERNWINDOWMEX_H