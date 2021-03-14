#ifndef BMP_H
#define BMP_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <time.h>
#include <unistd.h>

typedef struct{
    int height;
    int width;
    int fileSize;
    int headerSize;
    int bitsPerPixel;
    int bytesPerPixel;
    int pixel_bytes;
    unsigned char *header;
    unsigned char *pixels;

}BMP;


BMP *getImageInfo(char *source_file);
void createOutput(BMP *image, int f_text, char *text, int f_date, unsigned char *color, int pos[], char *output_file);
char *getDate();


#endif