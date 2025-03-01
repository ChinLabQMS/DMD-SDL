#include "BaseWindow.h"
#include "Benchmark.h"

int main(int argc, char* argv[]){
    BaseWindow window;
    
    // Measure execution time of instance (non-static) methods
    measureExecutionTime("Initialize SDL", &BaseWindow::init, window, true);
    measureExecutionTime("Open window", &BaseWindow::open, window, true);
    measureExecutionTime("Display color", &BaseWindow::displayColor, window, 255, 0, 0, true);
    
    // Set static pattern
    measureExecutionTime("Set static pattern path", &BaseWindow::setStaticPatternPath, window, (const char*) "../resources/text/CHIN.bmp", true);
    measureExecutionTime("Set static pattern path", &BaseWindow::setStaticPatternPath, window, (const char*) "../resources/text/QMS.bmp", true);
    measureExecutionTime("Set static pattern path", &BaseWindow::setStaticPatternPath, window, (const char*) "../resources/text/RGB.bmp", true);
    SDL_Delay(2000);

    // Set dynamic pattern
    size_t pattern_size = window.getWindowWidth() * window.getWindowHeight();
    uint32_t * red = new uint32_t[pattern_size];
    uint32_t * green = new uint32_t[pattern_size];
    uint32_t * blue = new uint32_t[pattern_size];
    for (size_t i = 0; i < pattern_size; i++) {
        red[i] = 0xFFFF0000;
        green[i] = 0xFF00FF00;    
        blue[i] = 0xFF0000FF;
    }
    window.displayColor(0, 0, 0, false);
    measureExecutionTime("Set dynamic pattern to red", &BaseWindow::setDynamicPattern, window, (void*) red, false);
    measureExecutionTime("Set dynamic pattern to green", &BaseWindow::setDynamicPattern, window, (void*) green, false);
    measureExecutionTime("Set dynamic pattern to blue", &BaseWindow::setDynamicPattern, window, (void*) blue, false);
    measureExecutionTime("Set dynamic pattern to red", &BaseWindow::setDynamicPattern, window, (void*) red, false);
    measureExecutionTime("Set dynamic pattern to green", &BaseWindow::setDynamicPattern, window, (void*) green, false);
    measureExecutionTime("Set dynamic pattern to blue", &BaseWindow::setDynamicPattern, window, (void*) blue, false);
    measureExecutionTime("Set dynamic pattern to red", &BaseWindow::setDynamicPattern, window, (void*) red, false);
    measureExecutionTime("Set dynamic pattern to green", &BaseWindow::setDynamicPattern, window, (void*) green, false);
    measureExecutionTime("Set dynamic pattern to blue", &BaseWindow::setDynamicPattern, window, (void*) blue, false);
    measureExecutionTime("Set dynamic pattern to red", &BaseWindow::setDynamicPattern, window, (void*) red, false);
    measureExecutionTime("Set dynamic pattern to green", &BaseWindow::setDynamicPattern, window, (void*) green, false);
    measureExecutionTime("Set dynamic pattern to blue", &BaseWindow::setDynamicPattern, window, (void*) blue, false);
    delete[] green;
    delete[] red;
    delete[] blue;

    SDL_Delay(2000);
    measureExecutionTime("Close window", &BaseWindow::close, window, true);

    SDL_Delay(30000);

    return 0;
}
