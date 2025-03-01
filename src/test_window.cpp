#include "BaseWindow.h"
#include "Benchmark.h"

int main(int argc, char* argv[]){
    BaseWindow window;
    
    // Measure execution time of instance (non-static) methods
    measureExecutionTime("Initialize SDL", &BaseWindow::init, window, true);
    measureExecutionTime("Open window", &BaseWindow::open, window, true);
    measureExecutionTime("Display color", &BaseWindow::displayColor, window, 255, 0, 0, true);
    
    // Set static pattern
    measureExecutionTime("Set static pattern path", &BaseWindow::setStaticPatternPath, window, (const char*) "../resources/text/CHIN.bmp", false);
    measureExecutionTime("Set static pattern path", &BaseWindow::setStaticPatternPath, window, (const char*) "../resources/text/QMS.bmp", false);
    measureExecutionTime("Set static pattern path", &BaseWindow::setStaticPatternPath, window, (const char*) "../resources/text/RGB.bmp", false);
    SDL_Delay(2000);

    // Set dynamic pattern
    size_t pattern_size = window.getWindowWidth() * window.getWindowHeight();
    uint32_t** pattern = new uint32_t*[23];
    for (size_t i = 0; i < 23; i++) {
        pattern[i] = new uint32_t[pattern_size];
        uint32_t color = (~0U << (24 - (i + 1)));
        for (size_t j = 0; j < pattern_size; j++) {
            pattern[i][j] = color;
        }
    }
    for (size_t i = 0; i < 23; i++) {
        measureExecutionTime("Set dynamic pattern", &BaseWindow::setDynamicPattern, window, (void*) pattern[i], false);
    }
    SDL_Delay(2000);
    for (size_t i = 0; i < 23; i++) {
        measureExecutionTime("Set dynamic pattern", &BaseWindow::setDynamicPattern, window, (void*) pattern[i], false);
    }


    // Free memory
    for (size_t i = 0; i < 23; i++) {
        delete[] pattern[i];
    }
    delete[] pattern;    

    SDL_Delay(2000);
    measureExecutionTime("Close window", &BaseWindow::close, window, true);

    std::cout << "Test complete." << std::endl;
    std::cout << "Press any key to exit." << std::endl;
    std::cin.get();

    return 0;
}
