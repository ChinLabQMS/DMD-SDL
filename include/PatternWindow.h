#ifndef PATTERNWINDOW_H
#define PATTERNWINDOW_H

#include "PixelCanvas.h"
#include "BaseWindow.h"

class PatternWindow : public BaseWindow, public PixelCanvas {
    protected:
        std::vector<uint8_t> StaticPatternRGB;
    public:
        void open(std::string arrangement = "Diamond", bool use_parallel = true);
        void close();    
        void setStaticPatternPath(const char *filepath, bool use_parallel = true) override;
        void loadPatternMemoryFromFile(const char *filepath, bool use_parallel = true);
        void displayPatternMemory(bool use_parallel = true);
        
};

#endif // PATTERNWINDOW_H