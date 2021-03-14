# Project Title

Nowadays, digital signatures are more and more used to sign documents. This is problematic because anyone can copy a signature from a document to paste it on a new document, so digital signatures are not meaningful anymore.

The aim of this project is to create a program that will take a signature stored in BMP format and modify it to “watermark” the signature.

^[Digital Watermarking](https://en.wikipedia.org/wiki/Digital_watermarking)

The idea is the following. The program will be called wm for “watermark”. Executing the following command line:
```bash
$ wm sign.bmp --text "Hello" --date --color FFFFFE --pos 10 20 --o modsign.bmp
```
will create a new modsign.bmp image where on position 10,20 from the top left of the image, there will be written ”Hello” followed by the current date (the output of the date linux command) in morse code, where a dot is a pixel of color FFFFFE (nearly white), and a dash is a “line” made of 3 pixels. Space between dots and dashes is one background pixel. Space between letters is 3 pixels, space between 2 words is 5 pixels 3. If the resolution is high, pixels will be really small, and the line will be quite invisible. Only people who know it is there and where to look for it will find it.

Command line:
```bash
$ wm sign.bmp --text "Hello" --date --color FFFFFE --pos 10,20 --o modsign.bmp
```
works the following way:

``sign.bmp`` is the name of the file that will be modified

``--text`` "Hello" means there will be written “hello” in morse code on sign.bmp. Note that ``--text`` is an option.
If it is not present, it will not write any text

``--date`` the output of system command date will written on ``sign.bmp`` in morse code after the text (if option ``--text`` is present. Note that ``--date`` is an option. If it is not present, it will not
write the date.

``--pos`` will write the morse code at position x,y starting from the top left of the image. If ``--pos`` is not present it will write from the top left position of the image (0,0).

``--o modsign.bmp`` will write the output (--o) in file modsign.bmp. If no ``--o`` option is mentioned, output on the standard output (the user will need to write ``> modsign.bmp`` to output.

## Getting Started

There are 8 files inside the project folder: 4 of them are C files (bmp.c, convert.c, morse.c, main.c), 3 of them are header files (bmp.h, convert.h, morse.h) and 1 bmp file (sign.bmp).

The functions are declared in header files, source codes are written in C files.

### Prerequisites

To be able to compile program user should have a C compiler installed to his system.

### Compiling

To compile the program use this:
```bash
$ gcc main.c bmp.c convert.c morse.c -std=gnu99
```
Note: Do not use -std=c99. It can cause errors

If you compile the program with the command that we provide you will get "a.out" file

## Usage

To start the program you can use this:
```bash
$ ./a.out sign.bmp --text hello --date --pos 600,200 --o modsign.bmp
```
In the code that I provide above, “sign.bmp” is the source file that you want to watermark. “modsign.bmp” is the name of the output file that you want.

NOTE: In case of giving wrong file (not .bmp file) program will show error and automatically exit.

If you don’t provide text and date it will give error and exit. You should provide one of them:
```bash
$ ./a.out sign.bmp --text hello --pos 600,200 --o modsign.bmp
```
or
```bash
$ ./a.out sign.bmp --date --pos 600,200 --o modsign.bmp
```
Also if there is no color code provided by the user the default code will be ABCEDF (please use real color codes not some garbage values).

And if the user don’t give the position it will be automatically (0,0).

NOTE: If the user tries to give invalid position program will automatically exit.

### Exceptions

This program is consider use only for English language. Using letters that don’t exist in english alphabet could cause the result that you don’t want. So use only english letters.
