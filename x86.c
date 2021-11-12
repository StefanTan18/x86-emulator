#include <stdlib.h>
#include <stdio.h>

#define byte unsigned char
#define word unsigned short
byte mem[0x100000];

byte AL, AH, BH, BL, CH, CL, DH, DL;
word AX, BX, CX, DX;
word IP, FLAGS, SP;
word CS, DS, SS, ES;

int main(int argc, char * argv[]) {
    CS = 0x1000;
    DS = 0x1000;
    SS = 0x1000;

    byte *code = &mem[CS*16];
    byte *data = &mem[DS*16];
    byte *stack = &mem[SS*16];

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

    // Reads File into mem[CS*16+0x100]
    fread(code + 0x100, sizeof(mem), 1, fp);

    /*
    // Test to make sure the .com file is read properly
    for (int i = 0; i < 65; i++) {
        if (i % 16 == 0) {
            printf("\n");
        }
        if (mem[i] < 10) {
            printf("0%x ", mem[i]);
        }
        else {
            printf("%x ", mem[i]);
        }
    }
    printf("\n");
    */

    fclose(fp);

    // TESTING STUFF
    // CURRENTLY JUST PRINTS OUT THE FIRST 16HEX, COMPARE AGAINST the Terminal Command: % hexdump (name of .com file)
    byte codebyte;
    IP = 0x100;
    SP = 0xFFFE;

    int i = 0;
    while(i < 16) {
        codebyte = mem[CS * 0x10 + IP];
        IP++;
        printf("%x ", codebyte);
        i++;
    }
    printf("\n");

/*
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
*/

    return 0;
}