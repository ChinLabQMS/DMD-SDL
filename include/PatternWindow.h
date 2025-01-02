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
        BaseWindow();    
        ~BaseWindow();
        virtual void printf(const char* format, ...);
        void setDisplayIndex(int id);
        int getDisplayIndex();
        void open(int id = -1);
        void close();
        bool isWindowCreated();
        void displayColor(int r, int g, int b);
        void projectFromFile(const char* filename);
        void selectAndProject(const char* default_location);
    protected:
        SDL_Window *window;
        SDL_Renderer *renderer;
        SDL_DisplayID *displays;
        SDL_DisplayID display_id;
        SDL_DisplayMode *display_mode;
        int target_width, target_height, window_width, window_height, num_displays, display_index;
        void init();
};

class PatternWindow : public BaseWindow {
    public:
        PatternWindow();
        ~PatternWindow();
};

#endif // PATTERNWINDOW_H