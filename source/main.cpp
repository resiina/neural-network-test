#include <iostream>

#include "bitmap.h"

int main()
{
    Bitmap testBitmap;
    testBitmap.open("data/testBitmap.bmp");

    PixelMatrix pixels = testBitmap.toPixelMatrix();

    for (const auto & row : pixels)
    {
        for (const auto & pixel : row)
        {
            std::cout << std::to_string(pixel.blue) << std::endl;
        }
    }

    return 0;
}