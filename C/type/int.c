/**
 * Author: E.K.Jithendiran
 */
#include <stdio.h>
void main()
{
    // sizeof is a uniary operator
    // sizeof returns numerical size value in byte
    printf("Size of int : %d\n", sizeof(int)); // Size of int : 4
    // unsigned int range 0 to 4294967295
    // unsigned calculate by 2^(bits) - 1
    // size of int is 4 byte (32 bit) 8 bit is 1 byte
    // signed calculation it is calculated by 2's complement -(2^(n-1)) to (2^(n-1)-1)
    // range from -2147483648 to 2147483647

    // short size is 2
    printf("Size of short : %d\n", sizeof(short));     // Size of short : 2
    printf("Size of short : %d\n", sizeof(short int)); // Size of short : 2

    // long size of 8
    // %l is used to print long values
    printf("Size of long : %ld\n", sizeof(long int)); // Size of long : 8

    // signed default is signed allows negative value
    int a = -1;
    signed int b = -2; // both are same
    printf("%d , %d\n", a, b);

    // unsigned allows positive value
    unsigned int c = -2;
    printf("%d\n", c); //-2 (%d is used to print signed decimal value)

    // in below example over flow is occur

    // over flow -2 is 4294967295 -2+1 = 4294967294
    printf("%u\n", c); // 4294967294 (%u is used to print unsigned int)

    // over flow -2 is 4294967295 -1+1 = 4294967295
    printf("%u\n", a); // 4294967295

    printf("Over Flow Example ......\n");
    int d = 2147483647;
    printf("%d\n", d);     // last value
    printf("%d\n", d + 1); // over flow by 1 so start from signed min value
    printf("%d\n", d + 2); // over flow by 2 so add 1 more value from signed min value

    // Over flow example
    // for simple let consider unsigned 3 bit value
    // 2^3 | 2^2  2^1  2^0 -> 3bit 0 to 7
    //  0  |  0    0    0  -> 0
    //  0  |  0    0    1  -> 1
    //  0  |  0    1    0  -> 2
    //  0  |  0    1    1  -> 3
    //  0  |  1    0    0  -> 4
    //  0  |  1    0    1  -> 5
    //  0  |  1    1    0  -> 6
    //  0  |  1    1    1  -> 7
    // When we represent 8
    //  1  |  0    0    0  -> 8 here 1 is place in 2^3 that is over flow and that over flow part is neglated
    //        0    0    0  -> 0 here is after overflow part is removed answer

    printf("Long Long Int Size : %d \n", sizeof(long long int)); // Long Long Int Size : 8
}