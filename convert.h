#ifndef CONVERT_H
#define CONVERT_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

void stringToStrHex(unsigned char* input,unsigned char* output);
unsigned char * LittleBigEndiannes(unsigned char * string);
int HexToInt(unsigned char * hex);
char *hexToStr(char *hex);

#endif