#include <stdlib.h>
#include <stdio.h>

#define byte unsigned char
#define word unsigned short
byte mem[0x100000];

byte AL, AH, BL, BH, CL, CH, DL, DH;
word AX, BX, CX, DX;
word IP, FLAGS, SP, SI, DI, BP;
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
        printf("File Read: %s\n", filename);
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
    // TEST TO MAKE SURE FILE IS READ PROPERLY
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
    
    // Closes the file 
    fclose(fp);

    /*
    // TESTING CODEBYTE AND ITERATION
    // PRINTS OUT THE FIRST 16HEX, COMPARED AGAINST the Terminal Command: % hexdump (name of .com file)
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
    */

    byte codebyte;
    IP = 0x100;
    SP = 0xFFFE;

    // Flags
    FLAGS = 0b1111001000000010;
    byte ZF, SF, CF, DF, OF;
    ZF = (FLAGS & (1 << 6)) >> 6;
    SF = (FLAGS & (1 << 7)) >> 7;
    CF = (FLAGS & (1 << 0)) >> 0;
    DF = (FLAGS & (1 << 10)) >> 10;
    OF = (FLAGS & (1 << 11)) >> 11;

    /* TESTING FLAGS
    printf("ZF = %d, SF = %d, CF = %d, DF = %d\n", ZF, SF, CF, DF);
    */

    int counter = 1;
    int printReg = 1;

    int option = 0;

    printf("    ********************** Welcome! *********************\n");
    printf("    *                    x86 Emulator                   *\n");
    printf("    *                                                   *\n");
    printf("    * 1. Run Program Normally                           *\n");
    printf("    * 2. Run Program Normally with Registers Printed    *\n");
    printf("    * 3. Quit                                           *\n");
    printf("    *****************************************************\n");
    printf("Please enter an option (1-3): ");

    do {
        scanf("%d", &option);
        printf("\n");

        switch (option) {
            case 1:
                printReg = 0;
            case 2:
                while (1) {

                    // PRINTING REGISTERS AND POINTERS AND FLAGS
                    if (printReg) {
                        printf("%4d IP=%04x AX=%04x BX=%04x CX=%04x DX=%04x SI=%04x DI=%04x BP=%04x  ZF=%d SF=%d CF=%d OF=%d\n", counter, IP, AX, BX, CX, DX, SI, DI, BP, ZF, SF, CF, OF);
                    }

                    byte result8 = 0;
                    word result16 = 0;
                    byte temp8 = 0;
                    word offset = 0;

                    // Reset the Flags
                    if (OF == 1) {
                        FLAGS = 0b1111101000000010; 
                    }
                    else {
                        FLAGS = 0b1111001000000010;
                    }
                    ZF = (FLAGS & (1 << 6)) >> 6;
                    SF = (FLAGS & (1 << 7)) >> 7;
                    CF = (FLAGS & (1 << 0)) >> 0;
                    DF = (FLAGS & (1 << 10)) >> 10;
                    OF = (FLAGS & (1 << 11)) >> 11;

                    codebyte = mem[CS * 0x10 + IP]; IP++;
                    switch (codebyte) {
                        case 0x00:
                            break;
                    
                        case 0x01:
                            break;

                        // ADD
                        case 0x02:
                            codebyte = mem[CS * 0x10 + IP]; IP++;
                            switch (codebyte) { 
                                case 0xC0:
                                    result8 = AL + AL;
                                    if (((result8 & (1 << 7)) >> 7) == 1) {
                                        FLAGS |= 1UL << 7;
                                        FLAGS |= 1UL << 11;
                                    }
                                    if (result8 == 0) {
                                        FLAGS |= 1UL << 6;
                                        FLAGS |= 1UL << 0;
                                    }
                                    AL += AL;
                                    //printf("%x\n", AL);
                                    break;
                                
                                case 0xC1:
                                    result8 = AL + CL;
                                    if (((result8 & (1 << 7)) >> 7) == 1) {
                                        FLAGS |= 1UL << 7;
                                        FLAGS |= 1UL << 11;
                                    }
                                    if (result8 == 0) {
                                        FLAGS |= 1UL << 6;
                                        FLAGS |= 1UL << 0;
                                    }
                                    AL += CL;
                                    break;
                                
                                case 0xC2:
                                    result8 = AL + DL;
                                    if (((result8 & (1 << 7)) >> 7) == 1) {
                                        FLAGS |= 1UL << 7;
                                        FLAGS |= 1UL << 11;
                                    }
                                    if (result8 == 0) {
                                        FLAGS |= 1UL << 6;
                                        FLAGS |= 1UL << 0;
                                    }
                                    AL += DL;
                                    break;
                                
                                case 0xC3:
                                    result8 = AL + BL;
                                    if (((result8 & (1 << 7)) >> 7) == 1) {
                                        FLAGS |= 1UL << 7;
                                        FLAGS |= 1UL << 11;
                                    }
                                    if (result8 == 0) {
                                        FLAGS |= 1UL << 6;
                                        FLAGS |= 1UL << 0;
                                    }
                                    AL += BL;
                                    break;
                                
                                case 0xC8:
                                    result8 = CL + AL;
                                    if (((result8 & (1 << 7)) >> 7) == 1) {
                                        FLAGS |= 1UL << 7;
                                        FLAGS |= 1UL << 11;
                                    }
                                    if (result8 == 0) {
                                        FLAGS |= 1UL << 6;
                                        FLAGS |= 1UL << 0;
                                    }
                                    CL += AL;
                                    break;
                                
                                case 0xC9:
                                    result8 = CL + CL;
                                    if (((result8 & (1 << 7)) >> 7) == 1) {
                                        FLAGS |= 1UL << 7;
                                        FLAGS |= 1UL << 11;
                                    }
                                    if (result8 == 0) {
                                        FLAGS |= 1UL << 6;
                                        FLAGS |= 1UL << 0;
                                    }
                                    CL += CL;
                                    break;

                                case 0xCA:
                                    result8 = CL + DL;
                                    if (((result8 & (1 << 7)) >> 7) == 1) {
                                        FLAGS |= 1UL << 7;
                                        FLAGS |= 1UL << 11;
                                    }
                                    if (result8 == 0) {
                                        FLAGS |= 1UL << 6;
                                        FLAGS |= 1UL << 0;
                                    }
                                    CL += DL;
                                    break;
                                
                                case 0xCB:
                                    result8 = CL + BL;
                                    if (((result8 & (1 << 7)) >> 7) == 1) {
                                        FLAGS |= 1UL << 7;
                                        FLAGS |= 1UL << 11;
                                    }
                                    if (result8 == 0) {
                                        FLAGS |= 1UL << 6;
                                        FLAGS |= 1UL << 0;
                                    }
                                    CL += BL;
                                    break;

                                case 0xD0:
                                    result8 = DL + AL;
                                    if (((result8 & (1 << 7)) >> 7) == 1) {
                                        FLAGS |= 1UL << 7;
                                        FLAGS |= 1UL << 11;
                                    }
                                    if (result8 == 0) {
                                        FLAGS |= 1UL << 6;
                                        FLAGS |= 1UL << 0;
                                    }
                                    DL += AL;
                                    break;

                                case 0xD1:
                                    result8 = DL + CL;
                                    if (((result8 & (1 << 7)) >> 7) == 1) {
                                        FLAGS |= 1UL << 7;
                                        FLAGS |= 1UL << 11;
                                    }
                                    if (result8 == 0) {
                                        FLAGS |= 1UL << 6;
                                        FLAGS |= 1UL << 0;
                                    }
                                    DL += CL;
                                    break;

                                case 0xD2:
                                    result8 = DL + DL;
                                    if (((result8 & (1 << 7)) >> 7) == 1) {
                                        FLAGS |= 1UL << 7;
                                        FLAGS |= 1UL << 11;
                                    }
                                    if (result8 == 0) {
                                        FLAGS |= 1UL << 6;
                                        FLAGS |= 1UL << 0;
                                    }
                                    DL += DL;
                                    break;

                                case 0xD3:
                                    result8 = DL + BL;
                                    if (((result8 & (1 << 7)) >> 7) == 1) {
                                        FLAGS |= 1UL << 7;
                                        FLAGS |= 1UL << 11;
                                    }
                                    if (result8 == 0) {
                                        FLAGS |= 1UL << 6;
                                        FLAGS |= 1UL << 0;
                                    }
                                    DL += BL;
                                    break;
                                
                                case 0xD8:
                                    result8 = BL + AL;
                                    if (((result8 & (1 << 7)) >> 7) == 1) {
                                        FLAGS |= 1UL << 7;
                                        FLAGS |= 1UL << 11;
                                    }
                                    if (result8 == 0) {
                                        FLAGS |= 1UL << 6;
                                        FLAGS |= 1UL << 0;
                                    }
                                    BL += AL;
                                    break;
                                
                                case 0xD9:
                                    result8 = BL + CL;
                                    if (((result8 & (1 << 7)) >> 7) == 1) {
                                        FLAGS |= 1UL << 7;
                                        FLAGS |= 1UL << 11;
                                    }
                                    if (result8 == 0) {
                                        FLAGS |= 1UL << 6;
                                        FLAGS |= 1UL << 0;
                                    }
                                    BL += CL;
                                    break;

                                case 0xDA:
                                    result8 = BL + DL;
                                    if (((result8 & (1 << 7)) >> 7) == 1) {
                                        FLAGS |= 1UL << 7;
                                        FLAGS |= 1UL << 11;
                                    }
                                    if (result8 == 0) {
                                        FLAGS |= 1UL << 6;
                                        FLAGS |= 1UL << 0;
                                    }
                                    BL += DL;
                                    break;
                                
                                case 0xDB:
                                    result8 = BL + BL;
                                    if (((result8 & (1 << 7)) >> 7) == 1) {
                                        FLAGS |= 1UL << 7;
                                        FLAGS |= 1UL << 11;
                                    }
                                    if (result8 == 0) {
                                        FLAGS |= 1UL << 6;
                                        FLAGS |= 1UL << 0;
                                    }
                                    BL += BL;
                                    break;
                            }
                            break;
                        
                        // SUB
                        case 0x2B:
                            codebyte = mem[CS * 0x10 + IP]; IP++;
                            switch (codebyte) {
                                // SUB AX, AX
                                case 0xC0:
                                    AX = 0x100 * AH + AL;
                                    FLAGS |= 1UL << 6;
                                    AX -= AX;
                                    break;

                                // SUB CX, CX
                                case 0xc9:
                                    CX = 0x100 * CH + CL;
                                    FLAGS |= 1UL << 6;
                                    CX -= CX;
                                    break;

                                // SUB DX, DX
                                case 0xD2:
                                    DX = 0x100 * DH + DL;
                                    FLAGS |= 1UL << 6;
                                    DX -= DX;
                                    break;

                                // SUB BX, BX
                                case 0xDB:
                                    BX = 0x100 * BH + BL;
                                    FLAGS |= 1UL << 6;
                                    BX -= BX;
                                    break;
                            }
                            break;

                        // INC BX
                        case 0x43:
                            BX = 0x100 * BH + BL;
                            BX++;
                            BL = BX & 0xFF;
                            BH = (BX - BL) / 0x100;
                            break;

                        // DEC AX
                        case 0x48:
                            AX = 0x100 * AH + AL;
                            AX--;
                            AL = AX & 0xFF;
                            AH = (AX - AL) / 0x100;
                            break;

                        // DEC CX
                        case 0x49:
                            CX = 0x100 * CH + CL;
                            CX++;
                            CL = CX & 0xFF;
                            CH = (CX - CL) / 0x100;
                            break;

                        // DEC DX
                        case 0x4A:
                            DX = 0x100 * DH + DL;
                            DX++;
                            DL = DX & 0xFF;
                            DH = (DX - DL) / 0x100;
                            break;

                        // DEC BX
                        case 0x4B:
                            BX = 0x100 * BH + BL;
                            BX--;
                            BL = BX & 0xFF;
                            BH = (BX - BL) / 0x100;
                            break;
                            
                        // DEC SP
                        case 0x4C:
                            SP--;
                            break;

                        // XCHG
                        case 0x86:
                            codebyte = mem[CS * 0x10 + IP]; IP++;
                            switch (codebyte) {
                                // XCHG AL, AH
                                case 0xc4:
                                case 0xe0:
                                    temp8 = AL;
                                    AL = AH;
                                    AH = temp8;
                                    break;

                                // XCHG CL, CH
                                case 0xcd:
                                case 0xe9:
                                    temp8 = CL;
                                    CL = CH;
                                    CH = temp8;
                                    break;

                                // XCHG DL, DH
                                case 0xd6:
                                case 0xf2:
                                    temp8 = DL;
                                    DL = DH;
                                    DH = temp8;
                                    break;

                                // XCHG BL, BH
                                case 0xdf:
                                case 0xfb:
                                    temp8 = BL;
                                    BL = BH;
                                    BH = temp8;
                                    break;
                            }
                            break;

                        // MOV r/m8, r8
                        case 0x88:
                            codebyte = mem[CS * 0x10 + IP]; IP++;
                            switch (codebyte) {
                                case 0x87:
                                    BX = 0x100 * BH + BL;
                                    data[BX] = AL;
                                    break;
                            }
                            codebyte = mem[CS * 0x10 + IP]; IP++;
                            codebyte = mem[CS * 0x10 + IP]; IP++;
                            break;
                        
                        // MOV r8, r/m8
                        case 0x8A:
                            codebyte = mem[CS * 0x10 + IP]; IP++;
                            switch (codebyte) {
                                case 0x97:
                                    BX = 0x100 * BH + BL;
                                    DL = data[BX];
                                    break;
                            }
                            codebyte = mem[CS * 0x10 + IP]; IP++;
                            codebyte = mem[CS * 0x10 + IP]; IP++;
                            DL = data[BX];
                            break;

                        // NOP
                        case 0x90:
                            break;

                        // MOV AL
                        case 0xB0:
                            codebyte = mem[CS * 0x10 + IP]; IP++;
                            AL = codebyte;
                            //printf("%x\n", AL);
                            break;
                        
                        // MOV DL
                        case 0xB2:
                            codebyte = mem[CS * 0x10 + IP]; IP++;
                            DL = codebyte;
                            break;
                        
                        // MOV AH
                        case 0xB4:
                            codebyte = mem[CS * 0x10 + IP]; IP++;
                            AH = codebyte;
                            break;
                        
                        // MOV CX
                        case 0xB9:
                            codebyte = mem[CS * 0x10 + IP]; IP++;
                            CX = codebyte;
                            CL = CX & 0xFF;
                            CH = (CX - CL) / 0x100;
                            break;

                        // MOV BX
                        case 0xBB:
                            codebyte = mem[CS * 0x10 + IP]; IP++;
                            BX = codebyte;
                            BL = BX & 0xFF;
                            BH = (BX - BL) / 0x100;
                            break;

                        // INT
                        case 0xCD: 
                            codebyte = mem[CS * 0x10 + IP]; IP++;
                            switch (codebyte) {
                                case 0x20:
                                    printf("\n");
                                    exit(0);
                                
                                case 0x21:
                                    if (AH == 0) {
                                        printf("\n");
                                        exit(0);
                                    }
                                    else if (AH == 0x4C) {
                                        printf("\n");
                                        exit(AL);
                                    }
                                    else if (AH == 0x06) {
                                        if (DL != 0xFF) {
                                            printf("%c", DL);
                                        }
                                        else if (AL == 0x00) {
                                            FLAGS |= 1UL << 6;
                                        }
                                        else {
                                            FLAGS &= ~(1UL << 6);
                                        }
                                    }
                                    break;
                            }
                            break;

                        // LOOP
                        case 0xE2:
                            codebyte = mem[CS * 0x10 + IP]; IP++;
                            CX = 0x100 * CH + CL;
                            CX--;
                            CL = CX & 0xFF;
                            CH = (CX - CL) / 0x100;
                            if (CX > 0) {
                                offset = 0xFF - codebyte + 1;
                                IP -= offset;
                            }
                            break;

                        // JMP
                        case 0xEB:
                            codebyte = mem[CS * 0x10 + IP]; IP++;
                            offset = codebyte;
                            IP += offset;
                            break;

                        // NEG
                        case 0xF6:
                            codebyte = mem[CS * 0x10 + IP]; IP++;
                            switch (codebyte) {
                                case 0xD8:
                                    AL = ~AL;
                                    break;

                                case 0xD9:
                                    CL = ~CL;
                                    break;

                                case 0xDA:
                                    DL = ~DL;
                                    break;
                                
                                case 0xDB:
                                    BL = ~BL;
                                    break;
                            }
                            break;

                        // INC
                        case 0xFE:
                            codebyte = mem[CS * 0x10 + IP]; IP++;
                            switch (codebyte) {
                                case 0xC0:
                                    result8 = AL + 1;
                                    if (((result8 & (1 << 7)) >> 7) == 1) {
                                        FLAGS |= 1UL << 7;
                                        FLAGS |= 1UL << 11;
                                    }
                                    if (result8 == 0) {
                                        FLAGS |= 1UL << 6;
                                        FLAGS |= 1UL << 0;
                                    }
                                    AL++;
                                    break;

                                case 0xC1:
                                    result8 = CL + 1;
                                    if (((result8 & (1 << 7)) >> 7) == 1) {
                                        FLAGS |= 1UL << 7;
                                        FLAGS |= 1UL << 11;
                                    }
                                    if (result8 == 0) {
                                        FLAGS |= 1UL << 6;
                                        FLAGS |= 1UL << 0;
                                    }
                                    CL++;
                                    break;

                                case 0xC2:
                                    result8 = DL + 1;
                                    if (((result8 & (1 << 7)) >> 7) == 1) {
                                        FLAGS |= 1UL << 7;
                                        FLAGS |= 1UL << 11;
                                    }
                                    if (result8 == 0) {
                                        FLAGS |= 1UL << 6;
                                        FLAGS |= 1UL << 0;
                                    }
                                    DL++;
                                    break;
                                
                                case 0xC3:
                                    result8 = BL + 1;
                                    if (((result8 & (1 << 7)) >> 7) == 1) {
                                        FLAGS |= 1UL << 7;
                                        FLAGS |= 1UL << 11;
                                    }
                                    if (result8 == 0) {
                                        FLAGS |= 1UL << 6;
                                        FLAGS |= 1UL << 0;
                                    }
                                    BL++;
                                    break;
                            }

                        case 0xFF:
                            break;
                    }

                    // Update the registers and flags
                    AX = 0x100 * AH + AL;
                    BX = 0x100 * BH + BL;
                    CX = 0x100 * CH + CL;
                    DX = 0x100 * DH + DL;
                    ZF = (FLAGS & (1 << 6)) >> 6;
                    SF = (FLAGS & (1 << 7)) >> 7;
                    CF = (FLAGS & (1 << 0)) >> 0;
                    DF = (FLAGS & (1 << 10)) >> 10;
                    OF = (FLAGS & (1 << 11)) >> 11;
                    
                    // Update counter
                    counter++;
                }
                break;

            case 3:
                exit(0);

            default:
                printf("Invalid Option, please enter a number from 1-3: ");
        }
    } while (option < 1 || option > 3); // Checks to see if the menu option is valid

    return 0;
}