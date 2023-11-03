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

    // Poniter size is based on the compiler and architecture, Pointer store memory address
    // based on the type it will fetch data from memory using * 
}

/**
 * Size of poniter is different for compiler
 * 
 * Compiler             int             pointer
 * 16 bit               2 byte          2 byte
 * 32 bit               4 byte          4 byte
 * 64 bit               4 byte          8 byte
*/

// the compiler bit will be maximum of cpu architecture if cpu is 64 bit then it supports upto 64 bit compiler
// so why pointer size is based on compiler and compiler size is based on cpu address bus width (64)

/**
 * For calculation example consider 32 bit
 *  With 32 bit cpu it has 32 bit for data line, the 32 bit cpu can handle 2^(32) 4294967296 bits of data
 * 4294967296 bits = 4194304 Kilo bits = 4096 Mega bits = 4 Giga bits
 * 32 bit computer supports up to 4 Giga bits of data
*/

// To get address length
/**
 * If it is a 16 bit compiler and pointer size is 2 byte it can point up to 2^(16) = 65536 memory location
 * like wise other complier based on the bit the address poniting size also varies
 * To represent 65536 memory location 16 bit address requires 16 bit address can be store in 2 byte
*/


// ref: https://www.youtube.com/watch?v=fpnE6UAfbtU 

/**
 * To store 256 bit data
 *  256 can be represent as a 16 * 16 matrix
 *  To get a particular bit particular row and column need to be active, 
 *  for 16 x 16 matrix needed 32 wire to activate any particular bit
 *  32 wire for selecting particular cell
 *  1 write enable wire
 *  1 read enable wire
 *  1 wire for data in/out
 *  so for 256bit needed 35 wire in total
 * 
 * store the row address in 4bit number
 * store the column address in 4bit number
 * 
 * why 4 bit?
 *   needed 16 wire for row/column to represent 16 needed 4bit 2^4 = 16
 * 
 * to select a particular cell need to feed 4bit row address and 4bit column address
 * for 256bit address 8bit address, 1 data wire, 1 write enable wire and 1 read enable wire
 * so with the 256bit memory able to read/write of 8bit data simultaneously
 * 
 * for 256 bit ram  8bit data, 8bit address input, read enable and write enable wrire required
 * 
 * now a day computers are 64bit address
*/

/**
 * Summary
 * Let consider need to store 256bit data

 * 256 can be represent as 16 x 16 matrix address space (one address space can store 1 bit)
 * To access particular address space need to select particular row and column, in this 256 case we have 16 address for row and 16 address for column 16
 * 16 row address can be represent as 4 bit and 16 column address can be represent as 4 bit, total needed 8bit address for access particular row
 
 * For 256 bit memoery, address length is 8 bit

 In 256 bit memory pointer neede 8 bit or 1 byte to store address, this is 8 bit  
*/

// ref https://www.youtube.com/watch?v=_SkpnG571z8