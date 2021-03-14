#include "bmp.h"
#include "convert.h"
#include "morse.h"


BMP *getImageInfo(char *source_file){

    BMP *image = (BMP *)malloc(sizeof(BMP));

    FILE *pFile1;
    if((pFile1 = fopen(source_file, "rb")) == NULL){
        printf("Error occured while opening source file. Exiting program...\n");
        exit(5);
    }
    puts(source_file);

    int check_bmp;
    fread(&check_bmp, 1, 2, pFile1);
    // printf("%d  \n", check_bmp);
    if(check_bmp != 19778){
        printf("Please enter .bmp file as source file. Exiting program...\n");
        exit(6);
    }

    // Size of file
    char ch[10];
    unsigned char *res = (unsigned char*)calloc(29, sizeof(char));
    fread(ch, 1, 4, pFile1);
    stringToStrHex(ch, res);
    res = LittleBigEndiannes(res);
    image->fileSize = HexToInt(res);
    printf("size: %d\n", image->fileSize);    

    //Next 4 bytes reserved for application. Not Used
    fseek(pFile1, 4, SEEK_CUR);
    
    // Header size(BMP and DIB headers). Where image starts
    fread(ch, 1, 4, pFile1);
    stringToStrHex(ch, res);
    res = LittleBigEndiannes(res);
    image->headerSize = HexToInt(res);
    printf("where image starts: %d\n", image->headerSize);

    // Size of DIB header
	fseek(pFile1, 4, SEEK_CUR);

    // Image width
    fread(ch, 1, 4, pFile1);
    stringToStrHex(ch, res);
    res = LittleBigEndiannes(res);
    image->width = HexToInt(res);   
    printf("image width: %d\n", image->width);   

    // Image height
    fread(ch, 1, 4, pFile1);
    stringToStrHex(ch, res);
    res = LittleBigEndiannes(res);
    image->height = HexToInt(res);
    printf("image height: %d\n", image->height);

    // Number of color plane
    fseek(pFile1, 2, SEEK_CUR);

    //Number of bits per Pixel
    fread(ch, 1, 2, pFile1);
    stringToStrHex(ch, res);
    res = LittleBigEndiannes(res);
    image->bitsPerPixel = HexToInt(res);   
    printf("bitsPerPixel: %d\n", image->bitsPerPixel);
    
    //Number of bytes per Pixel
    image->bytesPerPixel = image->bitsPerPixel/8;
    printf("bytesPerPixel: %d\n", image->bytesPerPixel);

    // Sets position to where the image starts
    fseek(pFile1, image->headerSize, SEEK_SET);

    //Reading image pixels into char array 
    image->pixel_bytes = image->width*image->height*image->bytesPerPixel;
    printf("pixel bytes: %d\n", image->pixel_bytes);
    image->pixels = (unsigned char*)calloc(image->pixel_bytes+1, sizeof(unsigned char));
    fread(image->pixels, 1, image->pixel_bytes, pFile1);
    image->pixels[image->pixel_bytes] = '\0';

    // Reading image header into char array
    fseek(pFile1, 0, SEEK_SET);
    image->header = (unsigned char*)calloc(image->headerSize, sizeof(unsigned char));
    fread(image->header, image->headerSize, 1, pFile1);

    fclose(pFile1);

    return image;    
}

void createOutput(BMP *image, int f_text, char *text, int f_date, unsigned char *color, int pos[], char *output_file){

    FILE *pFile2;
    if((pFile2 = fopen(output_file, "wb")) == NULL){
        printf("Error occured while opening output file. Exiting program...\n");
        exit(7);
    }            

    int exact_p = ((pos[1] - 1)*image->width + pos[0]-1) * image->bytesPerPixel;
  
    char *morse_text = (char*)calloc((strlen(text) + strlen(getDate()))*6, sizeof(char));
    if(f_text){
        for (int i = 0; i < strlen(text); i++){
            strncat(morse_text, morseTable(text[i]), strlen(morseTable(text[i])));
            strncat(morse_text, "   ", 3);
        }
    }

    if(f_date){
        char *date = getDate();
        for (int i = 0; i < strlen(date); i++){
            if(date[i] == ' '){
                strncat(morse_text, "     ", 5);
            }
            else{
                strncat(morse_text, morseTable(date[i]), strlen(morseTable(date[i])));
                strncat(morse_text, "   ", 3);
            }
        }
            
    }    
    printf("morse text is: %s\n", morse_text);
    int morse_text_size = strlen(morse_text);

    fwrite(image->header, image->headerSize, 1, pFile2);

    color = LittleBigEndiannes(color);

    int hexa = HexToInt(color);

    fwrite(image->pixels, 1, exact_p, pFile2);
    int point=0, dash=0, space=0, space2=0;
    for (int i = 0; i < morse_text_size; i++){
        if(morse_text[i] == '.'){
            point++;
            fwrite(&hexa, 1, image->bytesPerPixel, pFile2);
        }
        else if (morse_text[i] == '-'){
            dash++;
            fwrite(&hexa, 1, image->bytesPerPixel, pFile2);
            fwrite(&hexa, 1, image->bytesPerPixel, pFile2);
            fwrite(&hexa, 1, image->bytesPerPixel, pFile2);
        }
        else if(morse_text[i] == ' '){
            space = ((dash * 3) + point + space2) * image->bytesPerPixel;
            fwrite(&(image->pixels[exact_p + space]), 1, image->bytesPerPixel, pFile2);
            space2++;
        }                    
    }
    space = ((dash * 3) + point + space2) * image->bytesPerPixel;
    int pixel_cont = exact_p + space;
    int pixel_left = image->pixel_bytes-pixel_cont;
    fwrite(&(image->pixels[pixel_cont]), 1, pixel_left, pFile2);

    fclose(pFile2);
}


//gets the output of linux command 'date'
char *getDate(){
	char * res = (char *)malloc(100 * sizeof(char));
	FILE * fp = popen("date", "r");
	fgets(res, 100, fp);
	res[strlen(res) - 1] = '\0';
	pclose(fp);
	return res;
}


