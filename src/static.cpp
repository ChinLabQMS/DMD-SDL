#include <string>
#include "PatternWindow.h"

int main(int argc, char* argv[]){
    PatternWindow window;
    window.open();

    const char *basePath = SDL_GetBasePath();
    std::string path(basePath);
    path = path.substr(0, path.length() - 4) + "resources\\";
    window.selectAndProject(path.c_str());
    
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
