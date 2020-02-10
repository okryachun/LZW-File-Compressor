/*Author: Oleg Kryachun
Date: 02/05/2020
Summary: This is a LZW compression algorithm implemented in C.
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <lzw.h>


static FILE *fp, *fp1;

int readFunc(void *context) {
	return fgetc(fp);;
}

void writeFunc(unsigned char c, void *context) {
    fputc((int)c, context);
}

int main(int argc, char *argv[]) {
    int (*rdFunc)(void *);
    rdFunc = readFunc;

    void (*wrFunc)(unsigned char, void*);
    wrFunc = writeFunc;


    if(argc == 1)
        printf("Please specify either '-encode' or '-decode'\n");
	//This is for encoding: Reading in normal text and encoding it
    else if(strcmp(argv[1], "-encode") == 0) {
        fp = fopen("input.txt", "r");	//Optional: change "input.txt" to different path to change input file
        fp1 = fopen("output.txt","w");	//Optional: change "output.txt" to change output file path
        lzwEncode(10, 24, rdFunc, wrFunc, fp1);
	//This is for decoding: Reading in encoded text and decoding it
    }else if(strcmp(argv[1], "-decode") == 0) {
        fp = fopen("output.txt", "r");	//Optional: If "output.txt" changes above, set the path to the same path as above
        fp1 = fopen("outputDecode.txt","w");	//Optional: change "outputDecode.txt" to change decoded output path
        lzwDecode(10, 24, rdFunc,wrFunc,fp1);
    } else
        printf("Please specify either '-encode' or '-decode'\n");

    fclose(fp1);
    fclose(fp);

    return 0;
}


