#ifndef BMP_HPP
#define BMP_HPP

//задаем макроопределения
#define DATA_OFFSET_OFFSET 0x000A
#define WIDTH_OFFSET 0x0012
#define HEIGHT_OFFSET 0x0016
#define BITS_PER_PIXEL_OFFSET 0x001C
#define HEADER_SIZE 14
#define INFO_HEADER_SIZE 40
#define NO_COMPRESSION 0
#define MAX_NUMBER_OF_COLORS 0
#define ALL_COLORS_REQUIRED 0
#define RESOLUTIONX 0x26
#define RESOLUTIONY 0x002A

//задаем синонимы
typedef unsigned int int32;
typedef short int16;
typedef unsigned char byte;

struct ImageData {
    byte *pixels;
    int32 width;
    int32 height;
    int32 bytesPerPixel;
    int32 resolutionX;
    int32 resolutionY;
};

//gaussian filter
class GaussianFilter{
public:
    void ApplyGaussianFilterSettings(byte *inputPixels, byte *outputPixels, int32 width, int32 height, int32 bytesPerPixel, int blurRadius) {

        for (int y = 0; y < height; ++y) {
            for (int x = 0; x < width; ++x) {
            int redSum = 0;
            int greenSum = 0;
            int blueSum = 0;
            int numPixels = 0;

            for (int j = -blurRadius; j <= blurRadius; ++j) {
                for (int i = -blurRadius; i <= blurRadius; ++i) {
                    int offsetX = x + i;
                    int offsetY = y + j;

                    // проверим что мы находимся в пределах изображения
                    if (offsetX >= 0 && offsetX < width && offsetY >= 0 && offsetY < height) {
                        int pixelOffset = (offsetY * width + offsetX) * bytesPerPixel;
                        redSum += inputPixels[pixelOffset];
                        greenSum += inputPixels[pixelOffset + 1];
                        blueSum += inputPixels[pixelOffset + 2];
                        numPixels++;
                    }
                }
            }

            // Вычисляем средний цвет размытого пикселя
            int pixelOffset = (y * width + x) * bytesPerPixel;
            outputPixels[pixelOffset] = (byte)(redSum / numPixels);
            outputPixels[pixelOffset + 1] = (byte)(greenSum / numPixels);
            outputPixels[pixelOffset + 2] = (byte)(blueSum / numPixels);
        }
        }
    }
};

#endif