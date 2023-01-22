#include <SDL3/SDL.h>
#include <iostream>

#ifndef TYPE
#define TYPE 1
#endif

const int LINE_STEP = 100;
const int RED = 0xAA;
const int GREEN = 0xAA;
const int BLUE = 0xAA;

SDL_Window* window = NULL;
SDL_Surface* screenSurface = NULL;
SDL_Rect rect;

int main( int argc, char* args[] ){

    // Set DPI awareness to avoid OS scaling
    SDL_SetHint( SDL_HINT_WINDOWS_DPI_AWARENESS, "permonitor" );	

	//Initialize SDL
	if( SDL_Init( SDL_INIT_VIDEO ) < 0 ){
		std::cout << "SDL could not initialize! SDL_Error: " << SDL_GetError() << std::endl;
        return -1;
	}

    int count = SDL_GetNumVideoDisplays();
    if (count < 0){
        std::cout << "No display connected!\n";
        return -1;
    }
    else std::cout << "Number of displays connected: " << count << std::endl;

    //Get display mode
    SDL_DisplayMode mode;
    SDL_GetDesktopDisplayMode( count - 1, & mode );
    std::cout << "Display mode of #" << count << " Display:\n\tw = " << mode.w
                << "\n\th = " << mode.h << "\n\trefreshrate = "
                << mode.refresh_rate << "\n\tpixel format = " << SDL_GetPixelFormatName( mode.format ) <<std::endl;

    //Get monitor bounds
    SDL_GetDisplayBounds( count - 1, & rect );
    std::cout << "Display bound of #" << count << " Display:\n\tx = " << rect.x << "\n\ty = " << rect.y 
                << "\n\tw = " << rect.w << "\n\th = " << rect.h << std::endl;

    //Create a full screen window
    window = SDL_CreateWindow( "Project window", 
                rect.x, rect.y, rect.w, rect.h, SDL_WINDOW_FULLSCREEN_DESKTOP );
    if( window == NULL ){
        std::cout << "Window could not be created! SDL_Error: " << SDL_GetError() << std::endl;
        return -1;
    }

    //Get window surface
    screenSurface = SDL_GetWindowSurface( window );
    std::cout << "Screen surface format:\n\tw = " << screenSurface->w << "\n\th = " 
                << screenSurface->h << "\n\tpixel format = " << SDL_GetPixelFormatName( screenSurface->format->format ) 
                << "\n\tbits per pixel = " << unsigned(screenSurface->format->BitsPerPixel) << std::endl;

    #if TYPE == 1
        //Fill the surface
        SDL_FillSurfaceRect( screenSurface, NULL, SDL_MapRGB( screenSurface->format, RED, GREEN, BLUE ) );
    #else
        //Fill the surface with WHITE
        if (SDL_FillSurfaceRect( screenSurface, NULL, SDL_MapRGB( screenSurface->format, 0xFF, 0xFF, 0xFF ) ) < 0)
            std::cout << "Could not fill the window! SDL_Error: " << SDL_GetError() << std::endl;

        #if TYPE == 2
        //Add BLACK horizontal lines to the surface
        Uint32 BLACK = SDL_MapRGB(screenSurface->format, 0x00, 0x00, 0x00 );
        Uint32* data = (Uint32*) screenSurface->pixels;
        for ( int row = 0; row < rect.h; row += LINE_STEP )
            for (int col = 0; col < rect.w; col++ )
                data[ row * rect.w + col ] = BLACK;

        #elif TYPE == 3
        //Add BLACK vertical lines to the surface
        Uint32 BLACK = SDL_MapRGB(screenSurface->format, 0x00, 0x00, 0x00 );
        Uint32* data = (Uint32*) screenSurface->pixels;
        for ( int row = 0; row < rect.h; row++ )
            for (int col = 0; col < rect.w; col += LINE_STEP )
                data[ row * rect.w + col ] = BLACK;
        #endif
    #endif
        
    //Update the surface
    SDL_UpdateWindowSurface( window );

    //Hide cursor
    if (SDL_HideCursor() < 0) 
        std::cout << "Unable to hide cursor! SDL_Error: " << SDL_GetError() << std::endl;
        
    //Hack to get window to stay up
    SDL_Event e; bool quit = false; while( quit == false ){ while( SDL_PollEvent( &e ) ){ if( e.type == SDL_QUIT ) quit = true; } }

	//Destroy window
	SDL_DestroyWindow( window );

	//Quit SDL subsystems
	SDL_Quit();

	return 0;
}