#include <string>
#include "PatternWindow.h"

#ifdef TEST
const char* INIT_FILE_PATH = "docs\\example_1080x1920.bmp";
#else
const char* INIT_FILE_PATH = "docs\\example_912x1140.bmp";
#endif

int main(int argc, char* argv[]){
    PatternWindow window;
    window.open();
    window.setStaticPatternPath(INIT_FILE_PATH);
    
    // Event loop
    SDL_Event event;
    while (1) {
        SDL_PollEvent(&event);
        if (event.type == SDL_EVENT_QUIT) {
             break;
        }
    }
    return 0;
}
