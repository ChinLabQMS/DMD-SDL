#include <SDL3/SDL.h>
#include <iostream>

int main( int argc, char* args[] ){

    SDL_SetHint(SDL_HINT_WINDOWS_DPI_AWARENESS, "permonitor");

	//The window we'll be rendering to
	SDL_Window* window;
	
	//The surface contained by the window
	SDL_Surface* screenSurface;

	//Initialize SDL
	if( SDL_Init( SDL_INIT_VIDEO ) < 0 ){
		std::cout << "SDL could not initialize! SDL_Error: " << SDL_GetError() << "\n";
        return -1;
	}

    int count = SDL_GetNumVideoDisplays();
    if (count < 0){
        std::cout << "No display connected!\n";
        return -1;
    }
    else std::cout << "Number of display connected: " << count << "\n";

    //Get current display mode
    SDL_DisplayMode mode;
    SDL_GetDesktopDisplayMode(count-1, &mode);
    std::cout << "Display mode:\n\twidth = " << mode.w 
                << "\n\theight = " << mode.h << "\n\trefreshrate = "
                << mode.refresh_rate << "\n";

    //Create window
    window = SDL_CreateWindow( "Test window", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, mode.w, mode.h, SDL_WINDOW_FULLSCREEN );
    if( window == NULL ){
        printf( "Window could not be created! SDL_Error: %s\n", SDL_GetError() );
        return -1;
    }

    //Get window surface
    screenSurface = SDL_GetWindowSurface( window );
    std::cout << "Screen surface format:\n\twidth = " << screenSurface->w << "\n\theight = " << screenSurface->h << "\n";


    //Fill the surface white
    SDL_FillSurfaceRect( screenSurface, NULL, SDL_MapRGB( screenSurface->format, 0x00, 0x00, 0xFF ) );
        
    //Update the surface
    SDL_UpdateWindowSurface( window );
        
    //Hack to get window to stay up
    SDL_Event e; bool quit = false; while( quit == false ){ while( SDL_PollEvent( &e ) ){ if( e.type == SDL_QUIT ) quit = true; } }

	//Destroy window
	SDL_DestroyWindow( window );

	//Quit SDL subsystems
	SDL_Quit();

	return 0;
}
