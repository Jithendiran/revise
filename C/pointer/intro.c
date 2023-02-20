/**
 * Author: E.K.Jithendiran
 */
#include <stdio.h>
void main()
{
    // pointer will store initial  address of particular memory
    int a = 1, *p; // here a is int it has 4 byte it's value are store in 4 byte
    // 4 byte is 4 * 8 = 32 bit
    /**
     * Values are read from right to left it is based on encoding
     * assume a start address is 1001
     * addres       1004     1003     1002     1001
     * a = 1 -> 00000000 00000000 00000000 00000001
     */
    p = &a;                                                // this means p = 1001 // p will only store starting address
    printf("Address of a : %p, value of p : %p\n", &a, p); // Address of a : 0x7ffce2d2899c, value of p : 0x7ffce2d2899c
    printf("P points to value : %d\n", *p);                // P points to value : 1
    /*
     * '*' is dereference/ valueof operator which will retrive value from address
     * Based on the data type it will retrive value
     * In our example int will 4 byte when '*' is used it will access the value from start address stored in p to following 3 more byte
     */
    int b = 5005102;
    // 5005102 -> 00000000 01001100 01011111 00101110
    char *p1 = &b;
    printf("B value : %d\n", b);        // B value : 5005102
    printf("Value of *p1 : %d\n", *p1); // Value of *p1 : 46
                                        /**
                                         * In the above example *p1 is printed only 46 it's binary value is 0101110
                                         * It is the 1st 8 bit of b
                                         * char is 1 byte so it takes values from 1 byte
                                         */

    // suppose if *p1 value is changed it will affect the b value
    *p1 = 48;
    printf("Value of *p1 : %d , Value of b : %d\n", *p1, b); // Value of *p1 : 48 , Value of b : 5005104
    // 48 -> 00110000
    // 5005104 -> 00000000 01001100 01011111 00110000
}