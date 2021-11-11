#include <stdlib.h>
#include <stdio.h>

#define byte unsigned char
#define word unsigned short
byte mem[0x100000];

byte AL, AH, BH, BL, CH, CL, DH, DL;
word AX, BX, CX, DX;
word IP, FLAGS;
word CS, DS, SS, ES;

int main(int argc, char * argv[]) {
    CS = 0x1000;
    DS = 0x1000;
    SS = 0x1000;

    byte *code = mem[CS*16];
    byte *data = mem[DS*16];
    byte *stack = mem[SS*16];

    FILE *fp;
    char *filename;

    if (argc < 2) {
        printf("Missing name of .com file\n");
        return 0;
    }
    else {
        filename = argv[1];
        printf("Filename: %s\n", filename);
    }

    fp = fopen(filename, "rb");

    // File does not exist
    if (fp == NULL) {
        perror("The following error has occured");
        return 0;
    }

    // Read File
    char * line = NULL;
    size_t len = 0;
    ssize_t read;
    while ((read = getline(&line, &len, fp)) != -1) {
        printf("Retrieved line of length %zu:\n", read);
        printf("%s", line);
    }

    fclose(fp);

    byte codebyte;
    IP = 0x100;
    SP = 0xFFFE;

    while(1) {
        codebyte = mem[CS * 0x10 + IP]; IP++;
        switch (codebyte)
        {
        case 0x00:
            break;
        
        case 0x01:

        case 0x90:  // NOP
            break;
        
        default:
            break;
        }
    }

    return 0;
}