/**
 * Author: E.K.Jithendiran
 */
#include <stdio.h>
void main()
{
    /**
     * & ->  bitwise AND (binary)
     * | ->  bitwise OR (binary)
     * ~ ->  bitwise NOT (uniary)
     * << -> bitwise Left shift (binary)
     * >> -> bitwise Right shift (binary)
     * ^ ->  bitwise XOR (binary)
     */

    // AND
    int a = 7, b = 4;
    printf("a & b = %d\n", a & b); // a & b = 4

    // OR
    printf("a | b =%d\n", a | b); // a | b =7

    printf("~a = %d\n", ~a); //~a = -8

    if (a & b) // condition passed
    {
        printf("a & b condition\n"); // will print
    }
    if (1 & 2) // condition failed
    {
        printf("1 & 2 condition\n");
    }

    // Left shift
    // value (whose bit will be left shift) << no.of.bits to shift
    char c = 3;                         // used bcz char has 8 bit value stored;
    printf("3 << 1 is : %d\n", c << 1); // 3 << 1 is : 6
    /**
     * value of 3 is 0000 0011
     * 3 << 1
     *  -> 0000 011_ (trailing bit is empty)
     *  -> trailing bit is filled with 0
     *  -> 0000 0110 is 6
     */
    // left shift is equal to multiply by 2^rightoperand
    // 3 * 2^(1) -> 3 * 2 -> 6
    printf("3 << 2 is : %d\n", c << 2); // 3 << 2 is : 12

    // Right shift
    printf("3 >> 1 is : %d\n", c >> 1); // 3 >> 1 is : 1
    /**
     * value of 3 is 0000 0011
     * 3 >> 1
     *  -> _000 0001 (leading bit is empty)
     *  -> leading bit is filled with 0
     *  -> 0000 0001 is 1
     */
    // right shift is equal to division by 2^rightoperand
    // 3 / 2^(1) -> 3 / 2 -> 1
    printf("3 >> 2 is : %d\n", c >> 2); // 3 >> 2 is : 0

    // XOR -> Exclusive OR
    /**
     * 7 -> 0 1 1 1
     * 4 -> 0 1 0 0
     * --------------
     * 3 -> 0 0 1 1
     * --------------
     */
    printf("7 ^ 4 : %d\n", 7 ^ 4); // 7 ^ 4 : 3

    // using bit wise we can swap 2 values of variable (limitation overflow)
    printf("BF swap : a = %d, b = %d\n", a, b); // BF swap : a = 7, b = 4
    a = a ^ b;
    b = a ^ b;
    a = a ^ b;
    printf("AF swap : a = %d, b = %d\n", a, b); // AF swap : a = 4, b = 7

    signed char d = 128; // signed char range -128 to 127
    signed char e = 127;
    printf("BF XOR : d = %d, e = %d\n", d, e); // BF XOR : d = -128, e = 127
    d = d ^ e;
    e = d ^ e;
    d = d ^ e;
    printf("AF XOR : d = %d, e = %d\n", d, e); // AF XOR : d = 127, e = -128
}