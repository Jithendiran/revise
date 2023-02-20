/**
 * Author: E.K.Jithendiran
 */
#include <stdio.h>
void main()
{
    /**
     * void pointer is a pointer which has no associated data type with it
     * it can point to any data type and can be typecasted to any type
     *
     * cannot dereference a void pointer else it will lead to error
     */

    int n = 10;
    void *ptr;
    // printf("%d\n", *ptr); //invalid use of void expression
    ptr = &n;
    printf("%d\n", *(int *)ptr); // 10
    // in above eg ptr is typecasted to int pointer by (int *) then dereference value by *
    printf("Size of void %d, %d\n", sizeof(void), sizeof(void *)); // Size of void 1, 8
}