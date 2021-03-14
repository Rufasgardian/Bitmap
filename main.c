#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <getopt.h>
#include <string.h>
#include "bmp.h"

int main(int argc, char *argv[]) {

    // Flags for options in order to know they are used or not
    static int f_filename, f_text, f_date, f_color, f_pos, f_o;

    static int pos[2]; //default coordinates for position

    char *source_file;
    char *text;
    unsigned char *color = "ABCDEF";
    char *position;
    char *output_file; 

    BMP *source_image;

    int opt= 0;

    //Specifying the expected options
    //The two options l and b expect numbers as argument
    static struct option long_options[] = {
        {"text",   required_argument, 0,  't' },
        {"date",   no_argument,       0,  'd' },
        {"color",  required_argument, 0,  'c' },
        {"pos",    required_argument, 0,  'p' },
        {"o",      required_argument, 0,  'o' },
        {0,        0,                 0,  0   }
    };

    int long_index =0;
    while ((opt = getopt_long(argc, argv,"-:t:dc:p:o:", long_options, &long_index )) != -1) {
        switch (opt) {
            case 't' : 
                printf("regular argument '%s'\n", optarg);
                f_text = 1;
                text = (char*)calloc(strlen(optarg),sizeof(char));
                strcpy(text, optarg);
                break;
            case 'd' : 
                printf("regular argument date\n");
                f_date = 1;
                break;
            case 'c' : printf("regular argument '%s'\n", optarg);
                f_color=1;
                color = (char*)calloc(strlen(optarg),sizeof(char));
                strcpy(color, optarg); 
                break;
            case 'p' : printf("regular argument '%s'\n", optarg);
                f_pos = 1;
                position = (char*)calloc(strlen(optarg),sizeof(char));
                strcpy(position, optarg); 
                break;
            case 'o' : 
                printf("regular argument '%s'\n", optarg);
                f_o = 1;
                output_file = (char*)calloc(strlen(optarg),sizeof(char));
                strcpy(output_file, optarg);                
                break;                 
            case  1  :
                if(!f_filename){
                    printf("Non-option arg: %s\n", optarg);
                    f_filename = 1;
                    source_file = (char*)calloc(strlen(optarg),sizeof(char));
                    strcpy(source_file, optarg);                    
                }
                break;
            case '?' : printf("Unknown option: %c\n", optopt);
                break;
            case ':' : printf("Missing arg for %c. Exiting...\n", optopt);
                exit(1);
                break;
        }
    }

    if(!f_filename){
        printf("You didn't enter name of the source file. Exiting program...\n");
        exit(2);
    }

    if (!f_o){
        output_file = (char*)calloc(50,sizeof(char));
        printf("Please enter name of output file:\n");
        scanf("%s", output_file);
    }

    if (f_pos){
        char *token = strtok(position, ",");
        int i = 0;
        while( token != NULL ){
            f_pos++;
            pos[i++] = atoi(token);
            printf( "%d\n", pos[i-1] );
            token = strtok(NULL, ",");
        }
        if(f_pos>=4 || f_pos==2){
            printf("Incorrect number of position entered. Exiting program...\n");
            exit(3);
        }    
    }
    
    printf("source file is %s\n", source_file);
    printf("output file is %s\n", output_file);
    if(f_text) printf("text is %s\n", text);
    if(f_color) printf("color is %s\n", color);
    
    if(!f_text && !f_date){
        printf("Neither text nor date options provided by user. Exiting program...\n");
        exit(4);
    }

    printf("\n");

    source_image = getImageInfo(source_file); 

    if(f_pos){
        if(pos[0] > source_image->width*source_image->bytesPerPixel || pos[1] > source_image->height*source_image->bytesPerPixel){
            printf("Invalid position entered. Exiting program...\n");
            exit(10);
        }
    }    
    
    char *date = getDate();
    puts(date);

    if(!f_text){
        text = "";
    }

    createOutput(source_image, f_text, text, f_date, color, pos, output_file);
    
    return 0;
}