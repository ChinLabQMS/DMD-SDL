#include <string>
#include "BaseWindow.h"

int main(int argc, char* argv[]){
    BaseWindow window;
    window.init();
    window.open();
    window.selectAndProject();
    window.printf("Finished selecting and projecting.");
    
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
