#include "convert.h"

void stringToStrHex(unsigned char* input,unsigned char* output){
    // Convert text to hex.
    int len = strlen(input);
    
    for (int i = 0, j = 0; i < len; ++i, j += 2)
        sprintf(output + j, "%02X", input[i] & 0xff);
}

unsigned char * LittleBigEndiannes(unsigned char * string){
    unsigned char *copy = (unsigned char*)calloc(strlen(string)+1,sizeof(char));
    for (int i = 0; i < strlen(string)-1; i += 2){
        copy[i] = string[strlen(string)-i-2];
        copy[i+1] = string[strlen(string)-i-1];
    }
    copy[strlen(string)] = '\0';
    return copy;
}

int HexToInt(unsigned char *hex){
    int num = (int)strtol(hex, NULL, 16);
    return num;
}

char *hexToStr(char *hex){
    int len = strlen(hex);
    char *string = (char*)calloc(len, sizeof(char));
    for (int i = 0, j = 0; j < len; ++i, j += 2) {
        int val[1];
        sscanf(hex + j, "%2X", val);
        string[i] = val[0];
        string[i + 1] = '\0';
    }
    return string;
}