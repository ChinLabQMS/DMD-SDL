#ifndef PATTERNWINDOW_H
#define PATTERNWINDOW_H

#include "PixelCanvas.h"
#include "BaseWindow.h"

class PatternWindow : public BaseWindow, public PixelCanvas {
    protected:
        std::vector<uint8_t> StaticPatternRGB;
        std::vector<uint32_t> StaticPatternReal;
        std::vector<uint8_t> StaticPatternRealRGB;
        void updateStaticPatternProperties(bool use_parallel);
    public:
        virtual void open(bool verbose) override;
        virtual void open2(std::string arrangement, bool verbose, bool use_parallel);
        virtual void setDisplayIndex(int idx, bool verbose) override;
        virtual void setStaticPatternPath(const char *filepath, bool verbose) override; 
        virtual void setStaticPatternPath2(const char *filepath, bool verbose, bool use_parallel); // Add additional parameter to control parallelism
        void loadPatternMemoryFromFile(const char *filepath, bool verbose, bool use_parallel);
        void selectAndLoadPatternMemory(const char *default_location, bool verbose, bool use_parallel);
        bool displayPatternMemory(const uint32_t * indices, size_t num_frames, uint32_t delay, bool verbose, bool use_parallel);
        bool projectBlackTweezerPattern(int num_tweezers, const double *x0, const double *y0, double r, bool use_parallel);
};

#endif // PATTERNWINDOW_H