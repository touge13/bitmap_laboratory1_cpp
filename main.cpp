#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>

#include "bmp.hpp"
#include "readImage.cpp"
#include "writeImage.cpp"

int main() {
    //работа со строками для названий файлов
    char fname[31] = "";
    printf("%s\n", "\nВведите имя исходного файла из папки inputImages: ");
    scanf("%30s", fname);

    char* inputPath = new char[strlen("inputImages/") + strlen(fname) + 1];
    strcat(inputPath, "inputImages/");
    strcat(inputPath, fname);

    char* outputPath = new char[strlen("outputImages/") + strlen(fname) + 1];
    strcat(outputPath, "outputImages/");
    strcat(outputPath, fname);

    //выбор действия с фоткой
    printf("\nВведите номер действия (одну цифру):\n");
    printf("1. Duplication\n2. Turn right\n3. Turn left\n4. Reflected\n5. Flipped\n6. Gaussian filter\n");
    char action[10] = "";
    scanf("%10s", action);

    //чтение и запись файла путем вызова функции
    ImageData imageData;
    
    ReadImage(inputPath, &imageData.pixels, &imageData.width, &imageData.height, &imageData.bytesPerPixel, &imageData.resolutionX, &imageData.resolutionY);
    WriteImage(outputPath, imageData.pixels, imageData.width, imageData.height, imageData.bytesPerPixel, imageData.resolutionX, imageData.resolutionY, action);
    
    free(imageData.pixels);  //освобождение выделенной памяти
    return 0;
}