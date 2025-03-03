#include "PatternWindow.h"
#include "Benchmark.h"

int main() {
    PatternWindow window;
    
    measureExecutionTime("Open window", &PatternWindow::open, window, (std::string) "Diamond", true);
    measureExecutionTime("Set static pattern path", &PatternWindow::setStaticPatternPath, window, (const char *) "../resources/text/RGB.bmp", true);
    SDL_Delay(2000);
    measureExecutionTime("Close window", &PatternWindow::close, window);
    
    SDL_Delay(2000);

}
