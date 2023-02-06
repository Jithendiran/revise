#include<stdio.h>
int main() {
    // char is 1 byte
    // ASCII encoding  is 7 bit (signed char)
    char a = 'a'; // use single quotes '';
    //char b = "b"; // it will gives warning  warning: initialization of 'char' from 'char *' makes integer from pointer without a cast [-Wint-conversion]
    char c = 67;
    int d = 'D';
    printf("Char %c\n",a); // Char a
    printf("Int char %c, %c\n",c,d); // Int char C, D
    printf("Int %d\n",a);// Int 97
    // EASCII encoding is used for char it is 8 bit

    // for negative value left most bit is set to 1
    // Signed and unsigned
    // -128 is represented as 
    // -2^(7)  2^6 2^5 2^4 2^3 2^2 2^1 2^0
    //   1       0   0   0   0   0   0   0  -> -128

    // 128 is represented as 
    // 2^7  2^6 2^5 2^4 2^3 2^2 2^1 2^0
    //  1    0   0   0   0   0   0   0  -> 128

    // signed char range from -128 to 127
    char e = 128, f = -128; // 128 is overflowed to -128
    printf("%c : %c \n",e,f);// � : � 

    // unsigned char range from 0 to 255 uses EASCII
    unsigned char g = 0, h = 255;
    //unsigned char i = 266; //warning: unsigned conversion from 'int' to 'unsigned char' changes value from '266' to '10' [-Woverflow]
    printf("%c : %c \n",g,h);// : � 
}