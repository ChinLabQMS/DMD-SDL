#include <SDL3/SDL.h>
#include <iostream>

#ifndef TYPE
#define TYPE 1
#endif

const int LINE_STEP = 100;
const int FRAME_SAMPLE = 60;

SDL_Window* window = NULL;
SDL_Surface* surface = NULL;
SDL_Renderer* renderer = NULL;
SDL_Texture* texture = NULL;
Uint32* data = NULL;
SDL_DisplayMode mode;
SDL_Rect rect;
Uint64 timer_prev, timer_curr, timer_many, timer_exec;

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

void renderTexture(){
    SDL_UpdateTexture(texture, NULL, data, surface->pitch);
    SDL_RenderTexture(renderer, texture, NULL, NULL);
}

void saveBMP(const char* file){ 
    SDL_RenderReadPixels(renderer, NULL, surface->format->format, surface->pixels, surface->pitch);
    if (SDL_SaveBMP(surface, file)<0)
        std::cout << "Unable to save BMP! SDL_Error: " << SDL_GetError() << std::endl;
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
    SDL_GetDesktopDisplayMode(0, & mode);
    std::cout << "Display mode of #" << count << " Display:\n\tw = " << mode.w
                << "\n\th = " << mode.h << "\n\trefreshrate = "
                << mode.refresh_rate << "\n\tpixel format = " << SDL_GetPixelFormatName(mode.format) <<std::endl;

    //Get monitor bounds
    SDL_GetDisplayBounds(0, & rect);
    std::cout << "Display bound of #" << count << " Display:\n\tx = " << rect.x << "\n\ty = " << rect.y 
                << "\n\tw = " << rect.w << "\n\th = " << rect.h << std::endl;

    //Create a full screen window
    window = SDL_CreateWindow("Project window", 
                rect.x, rect.y, rect.w, rect.h, SDL_WINDOW_FULLSCREEN);
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
    renderer = SDL_CreateRenderer(window, NULL, SDL_RENDERER_PRESENTVSYNC | SDL_RENDERER_ACCELERATED);
    if (!renderer){
        std::cout << "Unable to create renderer! SDL_Error: " << SDL_GetError() << std::endl;
        return -1;
    }

    //Create a texture for rendering context
    texture = SDL_CreateTexture(renderer, surface->format->format, SDL_TEXTUREACCESS_STREAMING, rect.w, rect.h);
    data = (Uint32*) calloc(rect.w * rect.h, unsigned(surface->format->BytesPerPixel));

    return 0;
}

void projectPattern(){
    //Generate static testing patterns

    //Gray pattern
    #if TYPE == 1
    SDL_SetRenderDrawColor(renderer, 0xAA, 0xAA, 0xAA, SDL_ALPHA_OPAQUE);
    SDL_RenderClear(renderer);
    saveBMP("../resources/gray.bmp");
    #endif

    //Black pattern
    #if TYPE == 2
    SDL_SetRenderDrawColor(renderer, 0x00, 0x00, 0x00, SDL_ALPHA_OPAQUE);
    SDL_RenderClear(renderer);
    saveBMP("../resources/black.bmp");
    #endif

    //White pattern
    #if TYPE == 3
    SDL_SetRenderDrawColor(renderer, 0xFF, 0xFF, 0xFF, SDL_ALPHA_OPAQUE);
    SDL_RenderClear(renderer);
    saveBMP("../resources/white.bmp");
    #endif
    
    //Red pattern
    #if TYPE == 4
    SDL_SetRenderDrawColor(renderer, 0xFF, 0x00, 0x00, SDL_ALPHA_OPAQUE);
    SDL_RenderClear(renderer);
    saveBMP("../resources/red.bmp");
    #endif

    //Horizontal lines
    #if TYPE == 5
    printHoriLine(0);
    renderTexture();
    saveBMP("../resources/hori_line.bmp");
    #endif

    //Vertical lines
    #if TYPE == 6
    printVertLine(0);
    renderTexture();
    saveBMP("../resources/vert_line.bmp");
    #endif

    //Horizontal lines 2
    #if TYPE == 7
    printHoriLine(0);
    printHoriLine(1);
    renderTexture();
    saveBMP("../resources/hori_line2.bmp");
    #endif

    //Vertical lines 2
    #if TYPE == 8
    printVertLine(0);
    printVertLine(1);
    renderTexture();
    saveBMP("../resources/vert_line2.bmp");
    #endif

    SDL_RenderPresent(renderer);
}

int quitWindow(){
    SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);
	SDL_Quit(); 
    return 0;
}

int main(int argc, char* args[]){
    initWindow();
    projectPattern();

    SDL_Event e;
    bool quit = false;
    int iter = 0;
    int missed = 0;
    timer_curr = SDL_GetTicks();
    timer_prev = SDL_GetTicks();
    timer_many = SDL_GetTicks();
    timer_exec = SDL_GetTicks();
    while (!quit){  
        timer_curr = SDL_GetTicks();

        iter++;
        SDL_PollEvent( &e );
        if( e.type == SDL_QUIT ) 
            quit = true;

        Uint64 frame_time = timer_curr - timer_prev;

        if ((iter % FRAME_SAMPLE == 0) && (iter > FRAME_SAMPLE)){
            std::cout << "Current frame rate (Hz):\t" << 1000./frame_time
                    << "\tAverage frame rate (Hz):\t" << 1000.*FRAME_SAMPLE/double(timer_curr - timer_many)
                    << "\tProcessing time for " << FRAME_SAMPLE << " frames (ms): " 
                    << (1.*timer_exec)
                    << std::endl;
            timer_many = timer_curr;
            timer_exec = 0;
        }

        if ((iter > FRAME_SAMPLE) and ((frame_time * mode.refresh_rate > 1300) or (frame_time * mode.refresh_rate) < 800)){
            std::cout << "Frame time (ms): " << frame_time << "\titer: " << iter << std::endl;
            missed++;
        }
        
        #if TYPE == 9
        flashWhiteBlack(iter);
        #endif

        timer_prev = timer_curr;
        timer_exec += SDL_GetTicks() - timer_curr;

        //Update the surface
        SDL_RenderPresent(renderer);
    }
	quitWindow();
    std::cout << "Frame time error rate: " << double(missed)/iter << std::endl;

	return 0;
}