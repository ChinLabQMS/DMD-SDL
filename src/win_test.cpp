#include <SDL3/SDL.h>
#include <iostream>

#ifndef TYPE
#define TYPE 1
#endif

const int LINE_STEP = 100;

SDL_Window* window = NULL;
SDL_Surface* surface = NULL;
SDL_Renderer* renderer = NULL;
SDL_Texture* texture = NULL;
Uint32* data = NULL;
SDL_DisplayMode mode;
SDL_Rect rect;

void printVertLine(int start){
    for (int row = 0; row < rect.h; row++)
        for (int col = start; col < rect.w; col += LINE_STEP)
            data[row * rect.w + col] = SDL_MapRGB(surface->format, 0xFF, 0xFF, 0xFF);
}

void printHoriLine(int start){
    for (int row = start; row < rect.h; row += LINE_STEP)
        for (int col = 0; col < rect.w; col++)
            data[row * rect.w + col] = SDL_MapRGB(surface->format, 0xFF, 0xFF, 0xFF);
}

void flashWhiteBlack(int iter){
    if (iter % 2 == 0)
        SDL_SetRenderDrawColor(renderer, 0x00, 0x00, 0x00, SDL_ALPHA_OPAQUE);
    else
        SDL_SetRenderDrawColor(renderer, 0xFF, 0xFF, 0xFF, SDL_ALPHA_OPAQUE);
    SDL_RenderClear(renderer);
}

int initWindow(){
    // Set DPI awareness to avoid OS scaling
    SDL_SetHint(SDL_HINT_WINDOWS_DPI_AWARENESS, "permonitor");	

	//Initialize SDL
	SDL_Init(SDL_INIT_VIDEO);

    int count = SDL_GetNumVideoDisplays();
    if (count < 0){
        std::cout << "No display connected!\n";
        return -1;
    }
    else std::cout << "Number of displays connected: " << count << std::endl;

    //Get display mode
    SDL_GetDesktopDisplayMode(count - 1, & mode);
    std::cout << "Display mode of #" << count << " Display:\n\tw = " << mode.w
                << "\n\th = " << mode.h << "\n\trefreshrate = "
                << mode.refresh_rate << "\n\tpixel format = " << SDL_GetPixelFormatName(mode.format) <<std::endl;

    //Get monitor bounds
    SDL_GetDisplayBounds(count - 1, & rect);
    std::cout << "Display bound of #" << count << " Display:\n\tx = " << rect.x << "\n\ty = " << rect.y 
                << "\n\tw = " << rect.w << "\n\th = " << rect.h << std::endl;

    //Create a full screen window
    window = SDL_CreateWindow("Project window", 
                rect.x, rect.y, rect.w, rect.h, SDL_WINDOW_FULLSCREEN_DESKTOP);
    if(!window){
        std::cout << "Window could not be created! SDL_Error: " << SDL_GetError() << std::endl;
        return -1;
    }

    //Get window surface
    surface = SDL_GetWindowSurface(window);
    std::cout << "Screen surface format:\n\tw = " << surface->w << "\n\th = " 
                << surface->h << "\n\tpixel format = " << SDL_GetPixelFormatName(surface->format->format) 
                << "\n\tbits per pixel = " << unsigned(surface->format->BitsPerPixel) << std::endl;
    
    //Hide cursor on window
    SDL_HideCursor();

    //Create a 2D rendering context for the surface
    renderer = SDL_CreateRenderer(window, NULL, SDL_RENDERER_PRESENTVSYNC);
    if (!renderer){
        std::cout << "Unable to create renderer! SDL_Error: " << SDL_GetError() << std::endl;
        return -1;
    }

    //Create a texture for rendering context
    texture = SDL_CreateTexture(renderer, surface->format->format, SDL_TEXTUREACCESS_STREAMING, rect.w, rect.h);
    data = (Uint32*) calloc(rect.w * rect.h, unsigned(surface->format->BytesPerPixel));

    //Generate static testing patterns
    #if TYPE == 1
    SDL_SetRenderDrawColor(renderer, 0xAA, 0xAA, 0xAA, SDL_ALPHA_OPAQUE);
    SDL_RenderClear(renderer);
    SDL_RenderPresent(renderer);
    return 0;
    #endif

    #if TYPE == 2 or TYPE == 4
    printHoriLine(0);
    #endif

    #if TYPE == 3 or TYPE == 4
    printVertLine(0);
    #endif

    #if TYPE == 5 or TYPE == 7
    printHoriLine(0);
    printHoriLine(1);
    #endif

    #if TYPE == 6 or TYPE == 7
    printVertLine(0);
    printVertLine(1);
    #endif

    SDL_UpdateTexture(texture, NULL, data, surface->pitch);
    SDL_RenderTexture(renderer, texture, NULL, NULL);
    SDL_RenderPresent(renderer);
    return 0;
}

int quitWindow(){
	SDL_DestroyWindow(window);
	SDL_Quit(); 
    return 0;
}

int main(int argc, char* args[]){
    initWindow();

    SDL_Event e;
    bool quit = false;
    int iter = 0;
    int time = 0;
    int current = 0;
    while (!quit){
        SDL_PollEvent( &e );
        if( e.type == SDL_QUIT ) quit = true;

        if ((iter % 100 == 0) & (iter / 100 < 11)){
            current = SDL_GetTicks();
            std::cout << "Elapsed time (ms): " << current-time << "\tCurrent iter: "<< iter << std::endl;
            time = current;
        }
        iter++;

        #if TYPE == 8
        flashWhiteBlack(iter);
        #endif

        //Update the surface
        SDL_RenderPresent(renderer);
    }

	quitWindow();

	return 0;
}