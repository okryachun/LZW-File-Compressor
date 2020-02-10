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
    else if(strcmp(argv[1], "-encode") == 0) {
        fp = fopen("input.txt", "r");
        fp1 = fopen("output.txt","w");
        lzwEncode(10, 24, rdFunc, wrFunc, fp1);
    }else if(strcmp(argv[1], "-decode") == 0) {
        fp = fopen("output.txt", "r");
        fp1 = fopen("outputDecode.txt","w");
        lzwDecode(10, 24, rdFunc,wrFunc,fp1);
    } else
        printf("Please specify either '-encode' or '-decode'\n");

    fclose(fp1);
    fclose(fp);

    return 0;
}


