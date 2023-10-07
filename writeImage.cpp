#include "bmp.hpp"
/* А вот тут уже без указателей, странно. Но уже 8 параметров, бьем антирекорды */
void WriteImage(const char *fileName, byte *pixels, int32 width, int32 height, int32 bytesPerPixel, int32 resolutionX, int32 resolutionY, char *action) {
    int32 fileSize;
    int paddedRowSize;

    // Открытие файла в двоичном режиме
    FILE *outputFile = fopen(fileName, "wb");
    // Проверка, открыт ли файл успешно
    if (outputFile == NULL) {
        printf("Не удалось открыть файл\n");
        return;
    }
    
    int32 newWidth;
    int32 newHeight;
    /* Нет, точно нет. Си-шные функции работы со строками убивают людей пачками. */
    if (strcmp (action, "2") == 0 || strcmp (action, "3") == 0) {
        // Вычисление новых размеров изображения после поворота на 90 градусов
        newWidth = height;
        newHeight = width;
        // Вычисление размеров строк и изображения с учетом выравнивания
        paddedRowSize = (((newWidth) * (bytesPerPixel) + 3) / 4) * 4;
        fileSize = paddedRowSize * newHeight + HEADER_SIZE + INFO_HEADER_SIZE;
    } else {
        // Вычисление размеров строк и изображения с учетом выравнивания
        paddedRowSize = (((width) * (bytesPerPixel) + 3) / 4) * 4;
        fileSize = paddedRowSize * height + HEADER_SIZE + INFO_HEADER_SIZE;
    }

    //*****HEADER************//
    // Запись сигнатуры
    const char *BM = "BM";
    fwrite(&BM[0], 1, 1, outputFile);
    fwrite(&BM[1], 1, 1, outputFile);
    // Запись размера файла
    fwrite(&fileSize, 4, 1, outputFile);
    // Запись зарезервированных значений
    int32 reserved = 0x0000;
    fwrite(&reserved, 4, 1, outputFile);
    // Запись смещения данных
    int32 dataOffset = HEADER_SIZE + INFO_HEADER_SIZE;
    fwrite(&dataOffset, 4, 1, outputFile);

    //*******INFO*HEADER******//
    // Запись размера информационного заголовка
    int32 infoHeaderSize = INFO_HEADER_SIZE;
    fwrite(&infoHeaderSize, 4, 1, outputFile);

    if (strcmp (action, "2") == 0 || strcmp (action, "3") == 0) {
        // Запись новой ширины и высоты
        fwrite(&newWidth, 4, 1, outputFile);
        fwrite(&newHeight, 4, 1, outputFile);
    } else {
        // Запись новой ширины и высоты
        fwrite(&width, 4, 1, outputFile);
        fwrite(&height, 4, 1, outputFile);
    }
    /* Опять ручная запись данных, которые можно было просто скопировать из считываемой картинки. 
     * Опять напоминаю, что это превратилось бы вужас, если бы надо было обрабатывать 10 видов bmp*/
    // Запись количества плоскостей (всегда 1)
    int16 planes = 1;
    fwrite(&planes, 2, 1, outputFile);
    // Запись количества бит на пиксель
    int16 bitsPerPixel = bytesPerPixel * 8;
    fwrite(&bitsPerPixel, 2, 1, outputFile);
    // Запись типа сжатия
    int32 compression = NO_COMPRESSION;
    fwrite(&compression, 4, 1, outputFile);
    // Запись размера изображения
    int32 imageSize = height * width * bytesPerPixel;
    fwrite(&imageSize, 4, 1, outputFile);
    // Запись разрешения по горизонтали и вертикали
    fwrite(&resolutionX, 4, 1, outputFile); 
    fwrite(&resolutionY, 4, 1, outputFile);
    // Запись количества используемых цветов
    int32 colorsUsed = MAX_NUMBER_OF_COLORS;
    fwrite(&colorsUsed, 4, 1, outputFile);
    // Запись количества важных цветов
    int32 importantColors = ALL_COLORS_REQUIRED;
    fwrite(&importantColors, 4, 1, outputFile);

    // Запись данных
    /* А почему ты решил, что мы пишем программу для не-программиста? Как программисту,
     * который взял твою программу, использовать отдельно какую-то из этих функциональностей?
     * То, что находится в ветках, должно быть функциями, которые программист-пользователь сможет
     * удобно использовать */
    if (strcmp (action, "1") == 0) { //dublication
        int unpaddedRowSize = width * bytesPerPixel;
        for (int i = 0; i < height; i++) {
            //начинаем запись с начала последней строки массива пикселей
            int pixelOffset = ((height - i) - 1) * unpaddedRowSize;
            fwrite(&pixels[pixelOffset], 1, paddedRowSize, outputFile);	
        }

    } else if (strcmp (action, "2") == 0) { //turn right
        // Вычисление размера строк в оригинальном изображении
        int unpaddedRowSize = newHeight * bytesPerPixel;
        // Запись пикселей (столбец за столбцом)
        for (int i = 0; i < newHeight; i++) {
            for (int j = newWidth - 1; j >= 0; j--) {
                int pixelOffset = j * unpaddedRowSize + ((newHeight - i) - 1) * bytesPerPixel;
                fwrite(&pixels[pixelOffset], 1, bytesPerPixel, outputFile);
            }
        }

    } else if (strcmp (action, "3") == 0) { //turn left
        // Вычисление размера строк в оригинальном изображении
        int unpaddedRowSize = newHeight * bytesPerPixel;
        // Запись пикселей (столбец за столбцом)
        for (int i = 0; i < newHeight; i++) {
            for (int j = newWidth - 1; j >= 0; j--) {
                int pixelOffset = ((newWidth - j) - 1) * unpaddedRowSize + i * bytesPerPixel;
                fwrite(&pixels[pixelOffset], 1, bytesPerPixel, outputFile);
            }
        }

    } else if (strcmp (action, "4") == 0) { //reflected
        int unpaddedRowSize = width * bytesPerPixel;
        for (int i = height - 1; i >= 0; i--) {
            for (int j = width - 1; j >= 0; j--) {
                int pixelOffset = i * unpaddedRowSize + j * bytesPerPixel;
                fwrite(&pixels[pixelOffset], 1, bytesPerPixel, outputFile); 
            }
        }

    } else if (strcmp (action, "5") == 0) { //flipped
        int unpaddedRowSize = width * bytesPerPixel;
        for (int i = 0; i < height; i++) {
            for (int j = width - 1; j >= 0; j--) {
                int pixelOffset = i * unpaddedRowSize + j * bytesPerPixel;
                fwrite(&pixels[pixelOffset], 1, bytesPerPixel, outputFile); 
            }
        }
        
    } else if (strcmp (action, "6") == 0) { //Gaussian filter
        printf("\nНасколько сильный эффект блюра вы хотите получить (от 1 до 10)?:\n");
        int blurRadius;
        scanf("%d", &blurRadius);
        while (blurRadius > 10 || blurRadius < 0) {
            printf("от 1 до 10..\n");
            scanf("%d", &blurRadius);
        } 

        //blur
        byte *blurredPixels = new byte[width * height * bytesPerPixel];
        GaussianFilter gaus; //вытаскиваю класс из bmp.hpp
        gaus.ApplyGaussianFilterSettings(pixels, blurredPixels, width, height, bytesPerPixel, blurRadius);
        //write data
        int i = 0;
        int unpaddedRowSize = width * bytesPerPixel;
        for ( i = 0; i < height; i++) {
                //начинаем запись с начала последней строки массива пикселей
                int pixelOffset = ((height - i) - 1) * unpaddedRowSize;
                fwrite(&blurredPixels[pixelOffset], 1, paddedRowSize, outputFile);	
        }
        delete [] blurredPixels;
    }

    fclose(outputFile);
}
