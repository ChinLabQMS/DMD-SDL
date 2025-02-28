#ifndef PATTERNWINDOW_H
#define PATTERNWINDOW_H

#include "PixelCanvas.h"
#include "BaseWindow.h"

class PatternWindow : public BaseWindow, public PixelCanvas {
    public:
        PatternWindow();
        void open(std::string arrangement = "Diamond", bool use_parallel = true);
        void close();    
        void setStaticPatternPath(const char *filepath, bool use_parallel = true);    
        void projectDynamicPatternFromCanvas();
    
    protected:
        void updateCanvas2StaticPattern(bool use_parallel = true);    
        void updateCanvas2DynamicPattern(bool use_parallel = true);
    };

#endif // PATTERNWINDOW_H