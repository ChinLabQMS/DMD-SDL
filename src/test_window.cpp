#include <iostream>
#include <chrono>
#include "BaseWindow.h"

// Template function to measure execution time of a non-static class member function
template <typename ClassType, typename ReturnType, typename... Args>
void measureExecutionTime(const std::string& label, ReturnType (ClassType::*func)(Args...), ClassType& obj, Args&&... args) {
    auto start = std::chrono::high_resolution_clock::now();

    // Call the member function
    (obj.*func)(std::forward<Args>(args)...);

    auto end = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> elapsed = end - start;
    std::cout << label << " execution time: " << elapsed.count() << " seconds" << std::endl;
}

int main(int argc, char* argv[]){
    BaseWindow window;
    window.init();

    // Measure execution time of instance (non-static) methods
    measureExecutionTime("Open window", &BaseWindow::open, window, true);
    
    // SDL_Surface *surface = window.getStaticPatternSurface();
    // SDL_PixelFormat format = surface->format;
    // std::cout << "Pixel format: " << SDL_GetPixelFormatName(format) << std::endl;
    
    // uint32_t *pixels = (uint32_t*) surface->pixels;
    // std::cout << "First 100 pixels: ";
    // for (int i = 0; i < 100; i++) {
    //     std::cout << pixels[i] << " ";
    // }
    // std::cout << std::endl;

    // // Event loop
    // SDL_Event event;
    // while (1) {
    //     SDL_PollEvent(&event);
    //     if (event.type == SDL_EVENT_QUIT) {
    //          break;
    //     }
    // }
    
    return 0;
}
