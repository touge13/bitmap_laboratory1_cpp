#include "bmp.hpp"

//pixels: указатель на массив байтов. Он будет содержать данные пикселей
/* Зачем тут везде указатели? Они тут не нужны вообще. Копирование числа в 32 бита это не тяжелая операция. Тем более
 * указатель на указатель на писксели, зачем оно так, ты же в итоге передал ровно то же количество памяти? Убери все указатели 
 */
/* Причем если бы это было методом класса картинки, тут нужен был бы всего один аргумент, а не 7. В нормальной функции вообще 
 * не должно быть больше 4 аргументов. 
 */
void ReadImage(const char *fileName,byte **pixels, int32 *width, int32 *height, int32 *bytesPerPixel, int32 *resolutionX, int32 *resolutionY) {
        //открываем файл в бинарном режиме
        FILE *imageFile = fopen(fileName, "rb");
        if (!imageFile) {
                printf("Файл не найден\n");
                exit(1);
        }
        
        //чтение data offset
	/* У экземпляпа структуры все данные в памяти хранятся подряд. Если они так же
         * расположены в считываемых данных, можно заполнить это всё одним вызовом 
	 */
        int32 dataOffset;
        fseek(imageFile, DATA_OFFSET_OFFSET, SEEK_SET);
        fread(&dataOffset, 4, 1, imageFile);      
        //чтение resolutionX
        fseek(imageFile, RESOLUTIONX, SEEK_SET);
        fread(resolutionX, 4, 1, imageFile);
        //чтение resolutionY
        fseek(imageFile, RESOLUTIONY, SEEK_SET);
        fread(resolutionY, 4, 1, imageFile);
        //чтение ширины
        fseek(imageFile, WIDTH_OFFSET, SEEK_SET);
        fread(width, 4, 1, imageFile);
        //чтение высоты
        fseek(imageFile, HEIGHT_OFFSET, SEEK_SET);
        fread(height, 4, 1, imageFile);
        //чтение количества бита на пиксель
        int16 bitsPerPixel;
        fseek(imageFile, BITS_PER_PIXEL_OFFSET, SEEK_SET);
        fread(&bitsPerPixel, 2, 1, imageFile);

        //выделяем массив пикселей
        *bytesPerPixel = ((int32)bitsPerPixel) / 8;
        //Каждая строка дополняется так, чтобы ее длина была кратна 4 байтам
        //Рассчитываем размер дополненной строки в байтах
        int paddedRowSize = (((*width) * (*bytesPerPixel) + 3) / 4) * 4; 
        //Нас не интересуют дополненные байты, поэтому мы выделяем память для данных пикселей
        int unpaddedRowSize = (*width) * (*bytesPerPixel);
        //Общий размер данных пикселей в байтах
        int totalSize = paddedRowSize * (*height);
        *pixels = (byte*)malloc(totalSize);

        //Читаем строку данных пикселя
        //Данные дополняются и сохраняются снизу вверх
        int i = 0;
        //укажем на последнюю строку нашего массива пикселей (unpadded)
        byte *currentRowPointer = *pixels + ((*height - 1) * unpaddedRowSize);
        for (i = 0; i < *height; i++) {
                //помещаем курсор файла в следующую строку сверху вниз
	        fseek(imageFile, dataOffset + (i * paddedRowSize), SEEK_SET);
	        //читаем только байты unpaddedRowSize
	        fread(currentRowPointer, 1, unpaddedRowSize, imageFile);
	        //укажем на следующую строку (снизу вверх)
	        currentRowPointer -= unpaddedRowSize;
        }

        fclose(imageFile);
}
