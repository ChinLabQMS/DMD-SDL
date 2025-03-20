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
    TypedArray<uint32_t> StaticPatternMex = factory.createArray({0, 0}, 
                                              (uint32_t *) nullptr, (uint32_t *) nullptr);
    TypedArray<uint8_t> StaticPatternRGBMex = factory.createArray({0, 0, 3}, 
                                              (uint8_t *) nullptr, (uint8_t *) nullptr);
    TypedArray<uint32_t> StaticPatternRealMex = factory.createArray({0, 0}, 
                                              (uint32_t *) nullptr, (uint32_t *) nullptr);
    TypedArray<uint8_t> StaticPatternRealRGBMex = factory.createArray({0, 0, 3},
                                              (uint8_t *) nullptr, (uint8_t *) nullptr);                                      
    void checkArguments(ArgumentList outputs, ArgumentList inputs);
public:
    MexFunction();
    void printf(const char* format, ...) override;
    void warn(const char* format, ...) override;
    void error(const char* format, ...) override;
    void lock();
    void unlock();
    void setStaticPatternPath2(const char* filepath, bool verbose, bool use_parallel) override;
    CharArray getOperationMode();
    CharArray getBaseDirectory();
    CharArray getStaticPatternPath();
    StructArray getDisplayModes();
    TypedArray<uint32_t> getPatternCanvas();
    TypedArray<uint8_t> getPatternCanvasRGBMex(bool use_parallel);
    TypedArray<uint32_t> getRealCanvas();
    TypedArray<uint8_t> getRealCanvasRGBMex(bool use_parallel);
    TypedArray<double> getNumLoadedPatterns();
    TypedArray<uint32_t> getPatternMemory(int index);
    TypedArray<uint32_t> getPatternMemoryReal(int index, uint32_t background_color, bool use_parallel);
    TypedArray<uint8_t> getPatternMemoryRGBMex(int index, bool use_parallel);
    TypedArray<uint8_t> getPatternMemoryRealRGB(int index, uint32_t background_color, bool use_parallel);
    TypedArray<uint8_t> convertPattern2RGBMex(const TypedArray<uint32_t> pattern, bool use_parallel);
    TypedArray<uint32_t> convertRGB2PatternMex(const TypedArray<uint8_t> rgb, bool use_parallel);
    TypedArray<bool> displayPatternMemory(const TypedArray<double> indices, uint32_t delay, bool verbose, bool use_parallel);
    void operator()(ArgumentList outputs, ArgumentList inputs) override;
};

#endif // PATTERNWINDOWMEX_H