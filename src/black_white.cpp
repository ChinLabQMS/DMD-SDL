#include "BaseWindow.h"

int main(int argc, char* argv[]){
    BaseWindow window;
    window.init(true);
    window.open(true);
    
    // Event loop
    SDL_Event event;
    int n = 0;
    Uint64 start_time = SDL_GetTicks();
    while (1) {
        SDL_PollEvent(&event);
        if (event.type == SDL_EVENT_QUIT) {
            break;
        }
        if (n % 2 == 0) {
            window.displayColor(0, 0, 0, false);
        } else {
            window.displayColor(255, 255, 255, false);
        }
        if ((n + 1) % 60 == 0) {
            int elapsed_time = (int) (SDL_GetTicks() - start_time);
            start_time = SDL_GetTicks();
            window.printf("Frame #%d, elapsed time: %d ms, estimated frame rate: %f Hz", 
                        n + 1, elapsed_time, 60 * 1000.0 / elapsed_time);
        }
        n++;
    }
    return 0;
}
