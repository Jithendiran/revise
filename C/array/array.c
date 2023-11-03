/**
 * Author: E.K.Jithendiran
 */
#include <stdio.h>
void main()
{
    // 2 -> row, 3 -> column
    int a[2][3] = {1, 2, 3, 4, 5, 6};
    int a1[2][3] = {{1, 2, 3}, {4, 5, 6}};
    int a2[][2] = {1, 2, 3, 4}; // here row is optional

    // 3d
    int b[2][2][3] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12};
    int b1[2][2][3] = {
        {{1, 2, 3}, {4, 5, 6}},
        {{7, 8, 9}, {10, 11, 12}}};
    int b2[][2][2] = {1, 2, 3, 4, 5, 6}; // here also 1st position is optional

    // ** means double pointer
    // * -> 1st * refers to number of 1D array
    // * -> 2nd * refers to values of the array

    /**
     *---------|------------------|-----------------|
     * value   | 1   |   2 |    3 |   4 |   5 |  6  |
     * address |(100)|(104)| (108)|(112)|(116)|(120)|
     * --------|------------------|-----------------|
     *         |(100)              (112)
     *
     * let assume int has 4 bytes start address of array is 100
     * a refers to 100 and a+1 refers to 112
     *
     * *a refers to 100             **a -> 1
     * *a+1 refers to 104           *(*a)+1 -> 2
     * *a+2 refers to 108           *(*a)+2 -> 3
     *
     * *(a+1) refers to 112         **(a+1) -> 4
     * (*(a+1)+1) refers to 116     *(*(a+1)+1) -> 5
     * (*(a+1)+2) refers to 120     *(*(a+1)+2) -> 6
     */
    printf("a = %d : a+1 = %d\n", a, a + 1);             // a = 1663167488 : a+1 = 1663167500 a[0] has total 12 bytes means can able to store 3 elements like wise a[1]
    printf("*a = %d : *a+1 = %d\n", *a, (*a + 1));       // *a = 1663167488 : *a+1 = 1663167492
    printf("**a = %d : *(*a+1) = %d\n", **a, *(*a + 1)); // **a = 1 : *(*a+1) = 2

    // start doubt
    int **p = &a;
    printf("p = %d : p+1 = %d\n", p, p + 1);             // p = 1663167488 : p+1 = 1663167496
    printf("*p = %d : *p+1 = %d\n", *p, (*(p + 1)));     // *p = 1 : *p+1 = 3 // why 3 is *p is still pointer
    printf("Size of p = %d, *p = %d, **p = %d\n",sizeof(p), sizeof(*p), sizeof(**p)); // Size of p = 8, *p = 8, **p = 4
    // printf("**p = %d : *(*p+1) = %d\n", **p, *(*p + 1)); // Segmentation fault
    // end doubt

    int **p1 = a;
    printf("p1 = %d : p1+1 = %d\n", p1, p1 + 1);             // p1 = 1663167488 : p1+1 = 1663167496
    printf("*p1 = %d : *p1+1 = %d\n", *p1, (*(p1 + 1)));     // *p1 = 1 : *p1+1 = 3
    // printf("**p1 = %d : *(*p1+1) = %d\n", **p1, *(*p1 + 1)); // Segmentation fault
}