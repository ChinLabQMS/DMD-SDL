#ifndef PATTERNWINDOW_H
#define PATTERNWINDOW_H

#include "PixelCanvas.h"
#include "BaseWindow.h"

class PatternWindow : public BaseWindow, public PixelCanvas {
    protected:
        std::vector<uint8_t> StaticPatternRGB;
    public:
        PatternWindow(std::string arrangement = "Diamond");
        void open(bool verbose = true) override;
        void open(bool verbose, bool use_parallel); // Overload the open function with additional parameter
        void close(bool verbose = true) override;
        void setStaticPatternPath(const char *filepath, bool verbose = true) override; 
        virtual void setStaticPatternPath(const char *filepath, bool verbose, bool use_parallel); // Overload the setStaticPatternPath function with additional parameter
        void loadPatternMemory(const uint32_t *pattern, size_t num_elements) override;
        void loadPatternMemoryFromFile(const char *filepath, bool verbose = true, bool use_parallel = true);
        void selectAndLoadPatternMemory(const char *default_location = NULL, bool verbose = true, bool use_parallel = true);
        void displayPatternMemory(bool use_parallel = true);
        
};

#endif // PATTERNWINDOW_H