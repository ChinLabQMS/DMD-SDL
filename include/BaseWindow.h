#ifndef BASEWINDOW_H
#define BASEWINDOW_H

#include "SDL3/SDL.h"

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
        virtual void printf(const char* format, ...);
        virtual void error(const char* format, ...);
        virtual void warn(const char* format, ...);
        void init(bool verbose = true);
        void open(bool verbose = true);
        void close(bool verbose = true);
        bool isWindowCreated();
        bool isWindowMinimized();
        void displayColor(int r = 0, int g = 0, int b = 0, bool verbose = true);
        void setDynamicPattern(void* pattern, bool verbose = true);
        void selectAndProject(const char* default_location = NULL, bool verbose = true);
        void setDisplayIndex(int idx, bool verbose = true);
        void setStaticPatternPath(const char* filename, bool verbose = true);
        int getDisplayIndex();
        int getWindowHeight();
        int getWindowWidth();
        const char* getOperationMode();
        const char* getStaticPatternPath();
        const char* getBaseDirectory();
        SDL_Surface* getStaticPatternSurface();
    protected:
        SDL_Window *Window = NULL;
        SDL_Renderer *Renderer = NULL;
        SDL_Surface *StaticPatternSurface = NULL;
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