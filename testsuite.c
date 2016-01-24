#include <stdio.h>
#include <string.h>

/*

Version 1 of the TIFO stick software.
Its purpose is to write out hexadecimal values to stdout in binary.
This will simulate writing to the diods later in the project.

===============================================================================

What we want:

Nice, optimized code.

and

A program that
    - create strings where the first character is the letter thats 
      to be printed and the second character is a digit in range 1-5.
    - sends these strings as arguments to the printHex function
    - prints out the binary representation of the hex values.

===============================================================================

What we have:

Ugly code.

and

A program that:
    - create the strings.
    - can print out a hex number sent as argument to the printHex function.
    - CANT send a hex number as argument from the main function.

*/

#define		h1		0xff
#define		h2		0x10
#define		h3		0x10
#define		h4		0x10
#define		h5		0xff

#define		e1		0xff
#define		e2		0x91
#define		e3		0x91
#define		e4		0x91
#define		e5		0x91

#define		j1		0x06
#define		j2		0x01
#define		j3		0x01
#define		j4		0x01
#define		j5		0xfe

#define     sp     0x00


/* printHex writes out a specified hex value as a binary value 
   with 'X' representing 1 and '-' representing 0. */

void printHex(unsigned char val){
    while (val) {
        if (val & 1)
            printf("X");
        else
            printf("-");
    
        val >>= 1;
    }
    printf("\n");
}


int main()
{
    char string[3] = "hej", res[2], x[3];
    int i, j;
    char *ptr = string;


    for(i = 0; i < strlen(ptr); i++){
        res[1] = ptr[i];
        for(j = 1; j < 6; j++){
            res[2] = j;
            sprintf(x, "%c%d", res[1], res[2]);
            printHex(x);
        }
        printHex(sp);
        printHex(sp);
    }

    return(0);
}

