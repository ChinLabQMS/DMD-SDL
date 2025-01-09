#include <string>
#include <iostream>
#include "PatternWindow.h"

#if TEST
const char* INIT_FILE_PATH = "..\\docs\\red_1080x1920.bmp";
#else
const char* INIT_FILE_PATH = "..\\resources\\solid\\green.bmp";
#endif

int main(int argc, char* argv[]){
    PatternWindow window;
    window.setDisplayIndex(2);
    window.open();
    window.setStaticPatternPath(INIT_FILE_PATH);
    
    SDL_Surface *surface = window.getStaticPatternSurface();
    SDL_PixelFormat format = surface->format;
    std::cout << "Pixel format: " << SDL_GetPixelFormatName(format) << std::endl;
    
    uint32_t *pixels = (uint32_t*) surface->pixels;
    std::cout << "First 100 pixels: ";
    for (int i = 0; i < 100; i++) {
        std::cout << pixels[i] << " ";
    }
    std::cout << std::endl;

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
