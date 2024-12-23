#ifndef PATTERNWINDOW_H
#define PATTERNWINDOW_H

#include "SDL3/SDL.h"

#ifndef DEBUG
#define DEBUG 1
#endif

#if DEBUG
const int TARGET_DISPLAY_WIDTH = 1080;
const int TARGET_DISPLAY_HEIGHT = 1920;
#else
const int TARGET_DISPLAY_WIDTH = 912;
const int TARGET_DISPLAY_HEIGHT = 1140;
#endif

class PatternWindow {
    public:
        PatternWindow(int w, int h);
        ~PatternWindow();
        void init();
        void close();
        void pause(int time_ms);
        void loadTexture(const char* filename);
        void projectTexture();
        void selectFileAndProject(const char* default_location);
    private:
        SDL_Window *window;
        SDL_Renderer *renderer;
        SDL_Texture *texture;
        SDL_Surface *bitmap_surface;
        SDL_DisplayID display;
        int target_width, target_height, window_width, window_height;
};

#endif // PATTERNWINDOW_H