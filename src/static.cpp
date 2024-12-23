#include <string>
#include "PatternWindow.h"

int main(int argc, char* argv[])
{
    PatternWindow window(TARGET_DISPLAY_WIDTH, TARGET_DISPLAY_HEIGHT);
    window.init();

    char * basePath = (char *) SDL_GetBasePath();
    std::string path(basePath);
    path = path.substr(0, path.length() - 4) + "resources\\";
    window.selectFileAndProject(path.c_str());
    
    // Event loop
    SDL_Event event;
    bool running = true;
    while (running) {
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_EVENT_QUIT) {
                running = false;
                break;
            }
        }
    }

    window.close();
    window.pause(2000);

    return 0;
}
