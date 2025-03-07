#ifndef PATTERNWINDOW_H
#define PATTERNWINDOW_H

#include "PixelCanvas.h"
#include "BaseWindow.h"

class PatternWindow : public BaseWindow, public PixelCanvas {
    protected:
        std::vector<uint8_t> StaticPatternRGB;
    public:
        virtual void open(bool verbose) override;
        virtual void open2(std::string arrangement, bool verbose, bool use_parallel);
        virtual void close(bool verbose) override;
        virtual void setStaticPatternPath(const char *filepath, bool verbose) override; 
        virtual void setStaticPatternPath2(const char *filepath, bool verbose, bool use_parallel); // Add additional parameter to control parallelism
        void loadPatternMemoryFromFile(const char *filepath, bool verbose, bool use_parallel);
        void selectAndLoadPatternMemory(const char *default_location, bool verbose, bool use_parallel);
        void displayPatternMemory(int index, uint32_t delay, bool verbose, bool use_parallel);
};

#endif // PATTERNWINDOW_H