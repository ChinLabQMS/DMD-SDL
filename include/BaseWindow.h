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

const SDL_DialogFileFilter filters[] = {
    {"BMP images",  "bmp"},
    {"All files",   "*"}
};

void SDLCALL callback(void* userdata, const char* const* filelist, int filter);

class BaseWindow {
    public:
        ~BaseWindow();
        virtual void printf(const char* format, ...);
        virtual void error(const char* format, ...);
        virtual void warn(const char* format, ...);
        void init(bool verbose);
        virtual void open(bool verbose);
        virtual void close(bool verbose);
        void displayColor(int r, int g, int b, bool verbose);
        void readBMP(const char* filename, SDL_Surface **surface, bool verbose);
        void setDynamicPattern(void* pattern, bool verbose, bool use_parallel);
        void selectAndProject(const char* default_location, bool verbose);
        void setDisplayIndex(int idx, bool verbose);
        virtual void setStaticPatternPath(const char* filename, bool verbose);
        bool isWindowCreated() const;
        bool isWindowMinimized() const;
        int getWindowWidth() const;
        int getWindowHeight() const;
        void checkWindowState();
        void savePixelsAsBMP(const char* filename, void *pixels, int width, int height, int pitch, bool verbose);
        void selectAndSavePixelsAsBMP(const char* default_location, void *pixels, int width, int height, int pitch, bool verbose);
    protected:
        SDL_Window *Window = NULL;
        SDL_Renderer *Renderer = NULL;
        SDL_Surface *StaticPatternSurface = NULL;
        SDL_Surface *BMPSurface = NULL;
        SDL_Texture *Texture = NULL;
        SDL_DisplayID *Displays = NULL;
        SDL_DisplayID DisplayID;
        SDL_DisplayMode *DisplayMode = NULL;
        int WindowWidth = 0, WindowHeight = 0, NumDisplays, DisplayIndex;
        int ColorModeR = 0, ColorModeG = 0, ColorModeB = 0;
        char *BMPPath = NULL;
        char *StaticPatternPath = NULL;
        char *BaseDirectory = NULL;
        char *OperationMode = NULL;
};

#endif // BASEWINDOW_H