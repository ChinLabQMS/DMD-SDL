#include <SDL3/SDL.h>

class Window{
private:
    SDL_Window* window;
    SDL_Renderer* renderer;
    SDL_Texture* texture;

public:
    Window();
    void setWindowPattern(const int mode);
    void setWindowPattern(const char* file);
    void shutdown();
};