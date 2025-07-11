#include "PixelCanvas.h"
#include "Benchmark.h"

int main() {
    int width = 1140;
    int height = 912;
    PixelCanvas canvas;
    measureExecutionTime("Initialize canvas", &PixelCanvas::initCanvas, canvas, (int) width, (int) height, (std::string) "Diamond", true);

   // Allocate pixel data
    std::vector<uint32_t> pixels(width * height, 0xFF80C0E0);
    std::cout << "Pixel data created with size: " << pixels.size() << std::endl;

    // Measure execution time of static methods
    std::vector<uint8_t> rgb = measureExecutionTime("Convert pattern to RGB", &PixelCanvas::convertPattern2RGB, (const uint32_t*) pixels.data(), (int) height, (int) width, (int) width * 4, true);
    rgb = measureExecutionTime("Convert pattern to RGB (no parallel)", &PixelCanvas::convertPattern2RGB, (const uint32_t*) pixels.data(), (int) height, (int) width, (int) width * 4, false);
    std::vector<uint32_t> pattern = measureExecutionTime("Convert RGB to pattern", &PixelCanvas::convertRGB2Pattern, (const uint8_t*) rgb.data(), (int) height, (int) width, (int) width * 3, true);
    pattern = measureExecutionTime("Convert RGB to pattern (no parallel)", &PixelCanvas::convertRGB2Pattern, (const uint8_t*) rgb.data(), (int) height, (int) width, (int) width * 3, false);

    std::vector<uint8_t> pattern_rgb = measureExecutionTime("Get DynamicMemory RGB", &PixelCanvas::getDynamicMemoryRGB, canvas, 0, true);
    pattern_rgb = measureExecutionTime("Get DynamicMemory RGB (no parallel)", &PixelCanvas::getDynamicMemoryRGB, canvas, 0, false);
    
    std::cout << "Pattern[0] = " << pattern[0] << std::endl;
    std::cout << "Pixel[0] = " << pixels[0] << std::endl;
    std::cout << "RGB[0] = " << (uint32_t) rgb[0] << std::endl;
    std::cout << "RGB[1] = " << (uint32_t) rgb[1] << std::endl;
    std::cout << "RGB[2] = " << (uint32_t) rgb[2] << std::endl;

    measureExecutionTime("Reset dynamic memory", &PixelCanvas::resetDynamicMemory, canvas, (uint32_t) 0xFFFFFFFF, true);
    measureExecutionTime("Close canvas", &PixelCanvas::closeCanvas, canvas);
 
    // Reinitialize canvas with much smaller size
    measureExecutionTime("Initialize canvas", &PixelCanvas::initCanvas, canvas, 4, 4, (std::string) "Diamond", true);
    std::vector<uint32_t>& dynamic = canvas.DynamicMemory[0];
    std::cout << "RealNumRows = " << canvas.RealNumRows << std::endl;
    std::cout << "RealNumCols = " << canvas.RealNumCols << std::endl;

    std::vector<int> pattern_idx = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
    measureExecutionTime("Draw pixels", &PixelCanvas::drawPixelsDynamic, canvas, 0, (std::vector<int>) pattern_idx, (uint32_t) 0x000000FF, false);
    std::vector<uint32_t> real_pattern = measureExecutionTime("Convert pattern to real", 
        &PixelCanvas::convertPattern2Real, canvas,  (const uint32_t *) dynamic.data(), (uint32_t) 0x00FF0000, false);
    for (size_t i = 0; i < real_pattern.size(); i++) {
        int row = i / canvas.RealNumCols;
        int col = i % canvas.RealNumCols;
        std::cout << "Row = " << row << ", Col = " << col << std::endl;
        std::cout << "Real pattern[" << i << "] = " << real_pattern[i] << std::endl;
    }

    canvas.resetDynamicMemory(0xFFFFFFFF, true);
    std::vector<double> x0 = {0, 2, 4};
    std::vector<double> y0 = {0, 2, 4};
    std::vector<int> pixel_idx = canvas.drawCirclesOnReal(5, x0.data(), y0.data(), 1.0, true);
    std::cout<< "NumRows = " << canvas.NumRows << std::endl;
    std::cout<< "NumCols = " << canvas.NumCols << std::endl;
    std::cout << "Pixel indices drawn: ";
    for (size_t i = 0; i < pixel_idx.size(); i++) {
        int row = pixel_idx[i] / canvas.NumCols;
        int col = pixel_idx[i] % canvas.NumCols;
        int real_row = realDiamondX(canvas.NumRows, canvas.NumCols, row, col);
        int real_col = realDiamondY(canvas.NumRows, canvas.NumCols, row, col);
        std::cout << "(" << row << ", " << col << ") ";
        std::cout << "Real (" << real_row << ", " << real_col << ") ";
        std::cout << std::endl;
    }

    canvas.generateBlackTweezerPattern(3, x0.data(), y0.data(), 1.0, 0.0, 0.0, 1, 1, true);
    for (size_t i = 0; i < canvas.DynamicMemory.size(); i++) {
        std::vector<uint32_t>& pattern = canvas.DynamicMemory[i];
        std::vector<uint32_t> real_pattern = canvas.convertPattern2Real(pattern.data(), 0x000000FF, false);
        std::cout << std::endl;
        std::cout << "DynamicMemory[" << i << "] size: " << pattern.size() << std::endl;
        for (size_t j = 0; j < real_pattern.size(); j++) {
            int row = j / canvas.RealNumCols;
            int col = j % canvas.RealNumCols;
            std::cout << "Row = " << row << ", Col = " << col << std::endl;
            std::cout << "Dynamic pattern[" << i << "][" << j << "] = " << real_pattern[j] << std::endl;
        }
    }

    std::vector<double> calib = {0, 1, 1, 0, 3, 3};
    canvas.generateBlackTweezerPatternStatic(calib, x0, y0, 1);
    for (size_t i = 0; i < canvas.DynamicMemory.size(); i++) {
        std::vector<uint32_t>& pattern = canvas.DynamicMemory[i];
        std::vector<uint32_t> real_pattern = canvas.convertPattern2Real(pattern.data(), 0x000000FF, false);
        std::cout << std::endl;
        std::cout << "DynamicMemory[" << i << "] size: " << pattern.size() << std::endl;
        for (size_t j = 0; j < real_pattern.size(); j++) {
            int row = j / canvas.RealNumCols;
            int col = j % canvas.RealNumCols;
            std::cout << "Row = " << row << ", Col = " << col << std::endl;
            std::cout << "Dynamic pattern[" << i << "][" << j << "] = " << real_pattern[j] << std::endl;
        }
    }

    return 0;
}
