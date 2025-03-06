#include "PixelCanvas.h"
#include "Benchmark.h"

int main() {
    int width = 1140;
    int height = 912;
    PixelCanvas canvas("Diamond");
    measureExecutionTime("Initialize canvas", &PixelCanvas::initCanvas, canvas, (int) width, (int) height, (true));

    // Measure execution time of instance (non-static) methods
    measureExecutionTime("Reset pattern", &PixelCanvas::resetPattern, canvas, (uint32_t) 0xFF000000, true);
    measureExecutionTime("Reset pattern (no parallel)", &PixelCanvas::resetPattern, canvas, (uint32_t) 0xFF000000, false);
    measureExecutionTime("Reset background", &PixelCanvas::resetBackground, canvas, (uint32_t) 0xFF0000, true);
    measureExecutionTime("Reset background (no parallel)", &PixelCanvas::resetBackground, canvas, (uint32_t) 0xFF0000, false);
    measureExecutionTime("Update pattern to real", &PixelCanvas::updatePattern2Real, canvas, true);
    measureExecutionTime("Update pattern to real (no parallel)", &PixelCanvas::updatePattern2Real, canvas, false);
    measureExecutionTime("Update real to pattern", &PixelCanvas::updateReal2Pattern, canvas, true);
    measureExecutionTime("Update real to pattern (no parallel)", &PixelCanvas::updateReal2Pattern, canvas, false);

    // Allocate pixel data
    std::vector<uint32_t> pixels(width * height, 0xFF80C0E0);
    std::cout << "Pixel data created with size: " << pixels.size() << std::endl;

    measureExecutionTime("Load PatternMemory", &PixelCanvas::loadPatternMemory, canvas, (const uint32_t *) pixels.data(), (size_t) pixels.size());
    measureExecutionTime("Load PatternMemory", &PixelCanvas::loadPatternMemory, canvas, (const uint32_t *) pixels.data(), (size_t) pixels.size());
    measureExecutionTime("Load PatternMemory", &PixelCanvas::loadPatternMemory, canvas, (const uint32_t *) pixels.data(), (size_t) pixels.size());
    measureExecutionTime("Load PatternMemory", &PixelCanvas::loadPatternMemory, canvas, (const uint32_t *) pixels.data(), (size_t) pixels.size());
    measureExecutionTime("Clear PatternMemory", &PixelCanvas::clearPatternMemory, canvas);

    measureExecutionTime("Get PatternCanvas RGB", &PixelCanvas::getPatternCanvasRGB, canvas, true);
    measureExecutionTime("Get PatternCanvas RGB (no parallel)", &PixelCanvas::getPatternCanvasRGB, canvas, false);

    measureExecutionTime("Get RealCanvas RGB", &PixelCanvas::getRealCanvasRGB, canvas, true);
    measureExecutionTime("Get RealCanvas RGB (no parallel)", &PixelCanvas::getRealCanvasRGB, canvas, false);

    // Measure execution time of static methods
    std::vector<uint8_t> rgb = measureExecutionTime("Convert pattern to RGB", &PixelCanvas::convertPattern2RGB, (uint8_t *) pixels.data(), (int) height, (int) width, (int) width * 4, true);
    std::vector<uint32_t> pattern = measureExecutionTime("Convert RGB to pattern", &PixelCanvas::convertRGB2Pattern, (uint8_t *) rgb.data(), (int) height, (int) width, (int) width * 3, true);
    
    std::cout << "Pattern[0] = " << pattern[0] << std::endl;
    std::cout << "Pixel[0] = " << pixels[0] << std::endl;
    std::cout << "RGB[0] = " << (uint32_t) rgb[0] << std::endl;
    std::cout << "RGB[1] = " << (uint32_t) rgb[1] << std::endl;
    std::cout << "RGB[2] = " << (uint32_t) rgb[2] << std::endl;    

    measureExecutionTime("Close canvas", &PixelCanvas::closeCanvas, canvas);

    return 0;
}
