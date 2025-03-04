#ifndef BASEWINDOW_H
#define BASEWINDOW_H

#include "SDL3/SDL.h"
#include "omp.h"

#ifndef TEST
#define TEST 0
#endif

const int TARGET_DISPLAY_WIDTH = 912;
const int TARGET_DISPLAY_HEIGHT = 1140;

#if TEST
const int RENDERER_VSYNC = 0;
#else
const int RENDERER_VSYNC = 1;
#endif

class BaseWindow {
    public:
        ~BaseWindow();
        virtual void printf(const char* format, ...) const;
        virtual void error(const char* format, ...) const;
        virtual void warn(const char* format, ...) const;
        void init(bool verbose = true);
        virtual void open(bool verbose = true);
        virtual void close(bool verbose = true);
        void displayColor(int r = 0, int g = 0, int b = 0, bool verbose = true);
        void readBMP(const char* filename, SDL_Surface **surface, bool verbose = true);
        void setDynamicPattern(void* pattern, bool verbose = true, bool use_parallel = true);
        void selectAndProject(const char* default_location = NULL, bool verbose = true);
        void selectAndReadBMP(const char* default_location = NULL, bool verbose = true);
        void setDisplayIndex(int idx, bool verbose = true);
        virtual void setStaticPatternPath(const char* filename, bool verbose = true);
        bool isWindowCreated() const;
        bool isWindowMinimized() const;
        int getWindowHeight() const;
        int getWindowWidth() const;
    protected:
        SDL_Window *Window = NULL;
        SDL_Renderer *Renderer = NULL;
        SDL_Surface *StaticPatternSurface = NULL;
        SDL_Surface *BMPSurface = NULL;
        SDL_Texture *Texture = NULL;
        SDL_DisplayID *Displays = NULL;
        SDL_DisplayID DisplayID;
        SDL_DisplayMode *DisplayMode = NULL;
        int WindowWidth, WindowHeight, NumDisplays, DisplayIndex;
        int ColorModeR = 0, ColorModeG = 0, ColorModeB = 0;
        char *StaticPatternPath = NULL;
        char *BaseDirectory = NULL;
        char *OperationMode = NULL;
};

#endif // BASEWINDOW_H