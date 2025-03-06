#include "PixelCanvas.h"
#include "Benchmark.h"

int main() {
    int width = 1140;
    int height = 912;
    PixelCanvas canvas;
    measureExecutionTime("Initialize canvas", &PixelCanvas::initCanvas, canvas, (int) width, (int) height, (std::string) "Diamond", true);

    // Measure execution time of instance (non-static) methods
    measureExecutionTime("Reset pattern", &PixelCanvas::resetPattern, canvas, (uint32_t) 0xFF000000, true);
    measureExecutionTime("Reset pattern (no parallel)", &PixelCanvas::resetPattern, canvas, (uint32_t) 0xFF000000, false);
    measureExecutionTime("Reset background", &PixelCanvas::resetBackground, canvas, (uint32_t) 0xFF0000, true);
    measureExecutionTime("Reset background (no parallel)", &PixelCanvas::resetBackground, canvas, (uint32_t) 0xFF0000, false);

    // Allocate pixel data
    std::vector<uint32_t> pixels(width * height, 0xFF80C0E0);
    std::cout << "Pixel data created with size: " << pixels.size() << std::endl;

    measureExecutionTime("Load PatternMemory", &PixelCanvas::loadPatternMemory, canvas, (const uint32_t *) pixels.data(), (size_t) pixels.size());
    measureExecutionTime("Load PatternMemory", &PixelCanvas::loadPatternMemory, canvas, (const uint32_t *) pixels.data(), (size_t) pixels.size());
    measureExecutionTime("Load PatternMemory", &PixelCanvas::loadPatternMemory, canvas, (const uint32_t *) pixels.data(), (size_t) pixels.size());
    measureExecutionTime("Load PatternMemory", &PixelCanvas::loadPatternMemory, canvas, (const uint32_t *) pixels.data(), (size_t) pixels.size());
    measureExecutionTime("Clear PatternMemory", &PixelCanvas::clearPatternMemory, canvas);

    // Measure execution time of static methods
    std::vector<uint8_t> rgb = measureExecutionTime("Convert pattern to RGB", &PixelCanvas::convertPattern2RGB, (uint8_t *) pixels.data(), (int) height, (int) width, (int) width * 4, true);
    std::vector<uint32_t> pattern = measureExecutionTime("Convert RGB to pattern", &PixelCanvas::convertRGB2Pattern, (uint8_t *) rgb.data(), (int) height, (int) width, (int) width * 3, true);
    
    std::cout << "Pattern[0] = " << pattern[0] << std::endl;
    std::cout << "Pixel[0] = " << pixels[0] << std::endl;
    std::cout << "RGB[0] = " << (uint32_t) rgb[0] << std::endl;
    std::cout << "RGB[1] = " << (uint32_t) rgb[1] << std::endl;
    std::cout << "RGB[2] = " << (uint32_t) rgb[2] << std::endl;

    measureExecutionTime("Close canvas", &PixelCanvas::closeCanvas, canvas);

    // Reinitialize canvas
    measureExecutionTime("Initialize canvas", &PixelCanvas::initCanvas, canvas, 5, 5, (std::string) "Diamond", true);

    std::vector<int> real_idx = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9};
    measureExecutionTime("Draw pixels on real", &PixelCanvas::drawPixelsOnReal, canvas, (std::vector<int>) real_idx, (uint32_t) 0x000000FF, false);
    
    rgb = measureExecutionTime("Get PatternCanvas RGB", &PixelCanvas::getPatternCanvasRGB, canvas, true);
    rgb = measureExecutionTime("Get PatternCanvas RGB (no parallel)", &PixelCanvas::getPatternCanvasRGB, canvas, false);
    rgb = measureExecutionTime("Get RealCanvas RGB", &PixelCanvas::getRealCanvasRGB, canvas, true);
    rgb = measureExecutionTime("Get RealCanvas RGB (no parallel)", &PixelCanvas::getRealCanvasRGB, canvas, false);

    for (size_t i = 0; i < 10; ++i) {
        std::cout << "RealCanvas[" << i << "] = " << canvas.RealCanvas[i] << std::endl;
    }

    return 0;
}
