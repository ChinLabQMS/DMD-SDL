#ifndef PATTERNWINDOW_H
#define PATTERNWINDOW_H

#include "SDL3/SDL.h"

#ifndef TEST
#define TEST 1
#endif

#if TEST
const int TARGET_DISPLAY_WIDTH = 1080;
const int TARGET_DISPLAY_HEIGHT = 1920;
#else
const int TARGET_DISPLAY_WIDTH = 912;
const int TARGET_DISPLAY_HEIGHT = 1140;
#endif

class BaseWindow {
    public:
        ~BaseWindow();
        virtual void printf(const char* format, ...);
        virtual void error(const char* format, ...);
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
        bool getStaticMode();
        const char* getStaticPatternPath();
        const char* getBaseDirectory();
        SDL_Surface* getStaticPatternSurface();
        SDL_Surface* getDynamicPatternSurface();
        SDL_Renderer* getRenderer();
        SDL_Window* getWindow();
    protected:
        SDL_Window *Window = NULL;
        SDL_Renderer *Renderer = NULL;
        SDL_Surface *StaticPatternSurface = NULL;
        SDL_Surface *DynamicPatternSurface = NULL;
        SDL_DisplayID *Displays = NULL;
        SDL_DisplayID DisplayID;
        SDL_DisplayMode *DisplayMode;
        int WindowWidth, WindowHeight, NumDisplays, DisplayIndex;
        char *StaticPatternPath = NULL, *BaseDirectory = NULL;
        void init(bool verbose = true);
        // const char *getAbsoluteExistingFilePath(const char* filename);
};

class PatternWindow : public BaseWindow {
    public:
        PatternWindow(bool verbose = true);
        ~PatternWindow();
};

#endif // PATTERNWINDOW_H