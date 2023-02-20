/**
 * Author: E.K.Jithendiran
 */
#include <stdio.h>
// other than extern variable in global all are stored inside bss if not initlized. so only they have default value of 0
static int i;      // stored in bss segment in memory
static int i = 27; // now bss segment is now tansfered to data segment in memory. bcz value initlized is 27 other than 0
static int i;      // multiple declare is allowed but only one definition is allowed
// static int i =24; // Error already initlized
int j;
int j = 3;
// int j = 45;//error: redefinition of 'j'
// j = 34;//  error: redefinition of 'j' bzc assignment not done in global scope

int k;
int k = 34;
void main()
{
    int a = 052;                      // it is not a decimal value it is octal value
    printf("a in deciaml : %d\n", a); // a in deciaml : 42 (8^0 * 2 + 8^1 * 5) -> (2 + 40) -> 42
    printf("a in octal : %o\n", a);   // a in octal : 52

    int b = 0x43FF;                            // Hexa value
    printf("Decimal : %d, Hexa : %x\n", b, b); // Decimal : 17407, Hexa : 43ff
    int d = 0X43FF;                            // x -> X same and character used also not a case sensitive
    // in format sepcifier %x will print chaacter in small and %X will prit char in Caps
    printf("%x, %X\n", d, d); // 43ff, 43FF

    printf("i : %d, j: %d, K: %d\n", i, j, k);     // i : 27, j: 3, K: 34
    int k;                                         // this is stored inside bss segment
    j = 77;                                        // t is allowed
    printf("K af is : %d, j af is : %d \n", k, j); // K af is : 0, j af is : 77

    for (int i = 0; i < 20; i++)
    {
        switch (i)
        {
        case 0:
            i = i + 1;
        case 13:
            i = i + 5;
        case 2:
            i = i + 2;
        default:
            i = i + 4;
        }
        printf("i :  %d\n", i);
    }
    /**
     * Op
        i :  12
        i :  24
    */

    int i = 0;
    for (printf("one\n"); i < 3 && printf(""); i++)
    {
        printf("Hi\n");
    }
    // Op: one
    i = 0;
    for (printf("one\n"); i < 3 && printf("1\n"); i++)
    {
        printf("Hi\n");
    }
    /**
     * Op
        one
        1
        Hi
        1
        Hi
        1
        Hi
    */
}

// Reference https://www.youtube.com/watch?v=gegaS_gX3TY&list=PLBlnK6fEyqRggZZgYpPMUxdY1CYkZtARR&index=19