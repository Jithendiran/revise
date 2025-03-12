/**
 * Author: E.K.Jithendiran
 */
#include <stdio.h>
void main()
{
    int a = 1, *ptr = &a;  // ptr is a pointer to integer
    printf("a = %d\n", a); // a = 1
    a = 2;
    printf("a = %d\n", a); // a = 2
    *ptr = 3;
    printf("a = %d\n", a); // a = 3

    const int b = 2;       // const applied for int
    int *ptrb = &b;        // ptrb is a pointer to integer
    printf("b = %d\n", b); // b = 2
    // b = 3; error
    *ptrb = 3;
    printf("b = %d\n", b); // b = 3

    // ptrbc is a integer pointer whose value not change (const is applied to int which means value is not changeable)
    // const applied for int, not for pointer
    const int *ptrbc = &b;      // or int const *ptrbc = &b;
    printf("b = %d\n", *ptrbc); // b = 3
    //*ptrbc = 4;                 // error
    ptrbc = &a;
    printf("ptrbc = %d \n", *ptrbc); // ptrbc = 3

    // ptrd is an constant pointer whose address is not change (const is applied to ptrd which means address is not changeable )
    // const applied for pointer, not for integer
    int *const ptrd = &b;
    // ptrd = &a; //error
    printf("ptrd = %d \n", *ptrd); // ptrbc = 3
    *ptrd = 10;
    printf("B = %d\n", b); // B = 10

    // const applied for int and pointer
    // ptre is an constant pointer to constant integer whose address and value is not changeable
    const int *const ptre = &a;
    // ptre = &b; // error
    //*ptre = 20; // error
}