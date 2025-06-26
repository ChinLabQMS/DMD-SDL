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
protected:
    virtual void updateStaticPatternProperties(bool use_parallel) override; 
public:
    MexFunction();
    void printf(const char* format, ...) override;
    void warn(const char* format, ...) override;
    void error(const char* format, ...) override;
    void lock();
    void unlock();
    CharArray getOperationMode();
    CharArray getBaseDirectory();
    CharArray getStaticPatternPath();
    StructArray getDisplayModes();
    TypedArray<double> getDynamicMemorySize();
    TypedArray<double> getPatternMemorySize();
    TypedArray<uint32_t> getPatternMemory(int index);
    TypedArray<uint32_t> getPatternMemoryReal(int index, uint32_t background_color, bool use_parallel);
    TypedArray<uint8_t> getPatternMemoryRGB(int index, bool use_parallel);
    TypedArray<uint8_t> getPatternMemoryRealRGB(int index, uint32_t background_color, bool use_parallel);
    TypedArray<uint32_t> getDynamicMemory(int index);
    TypedArray<uint32_t> getDynamicMemoryReal(int index, uint32_t background_color, bool use_parallel);
    TypedArray<uint8_t> getDynamicMemoryRGB(int index, bool use_parallel);
    TypedArray<uint8_t> getDynamicMemoryRealRGB(int index, uint32_t background_color, bool use_parallel);
    TypedArray<uint8_t> convertPattern2RGBMex(const TypedArray<uint32_t> pattern, bool use_parallel);
    TypedArray<uint32_t> convertRGB2PatternMex(const TypedArray<uint8_t> rgb, bool use_parallel);
    TypedArray<bool> displayPatternMemory(const TypedArray<double> indices, uint32_t delay, bool verbose, bool use_parallel);
    TypedArray<bool> displayDynamicMemory(const TypedArray<double> indices, uint32_t delay, bool verbose, bool use_parallel);
    void generateBlackTweezerPattern(TypedArray<double> coords, 
                                     TypedArray<double> radius,
                                     TypedArray<double> shift,
                                     int num_RGB_buffers,
                                     int num_bin_frames, 
                                     bool use_parallel);
    void generateBlackTweezerPatternStatic(TypedArray<double> V, 
                                           TypedArray<double> R, 
                                           TypedArray<double> sites, 
                                           double radius);
    void operator()(ArgumentList outputs, ArgumentList inputs) override;
};

#endif // PATTERNWINDOWMEX_H