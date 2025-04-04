#ifndef PATTERNWINDOW_H
#define PATTERNWINDOW_H

#include "PixelCanvas.h"
#include "BaseWindow.h"

class PatternWindow : public BaseWindow, public PixelCanvas {
    protected:
        std::vector<uint8_t> StaticPatternRGB;
        std::vector<uint32_t> StaticPatternReal;
        std::vector<uint8_t> StaticPatternRealRGB;
    public:
        virtual void open(bool verbose) override;
        virtual void open2(std::string arrangement, bool verbose, bool use_parallel);
        virtual void close(bool verbose) override;
        virtual void setStaticPatternPath(const char *filepath, bool verbose) override; 
        virtual void setStaticPatternPath2(const char *filepath, bool verbose, bool use_parallel); // Add additional parameter to control parallelism
        void loadPatternMemoryFromFile(const char *filepath, bool verbose, bool use_parallel);
        void selectAndLoadPatternMemory(const char *default_location, bool verbose, bool use_parallel);
        bool displayPatternMemory(const uint32_t * indices, size_t num_frames, uint32_t delay, bool verbose, bool use_parallel);
        bool displayPatternCanvas(bool verbose, bool use_parallel);
        void savePatternAsBMP(const char *filepath, bool verbose);
        void saveRealAsBMP(const char *filepath, bool verbose);
        void savePatternMemoryAsBMP(size_t index, const char *filepath, bool verbose);
};

#endif // PATTERNWINDOW_H