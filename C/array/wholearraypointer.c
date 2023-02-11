#include <stdio.h>

void main()
{
    int a[] = {1, 2, 3, 4, 5};
    int *p = a;       // it points to 1st address of
    int(*p1)[5] = &a; // p1 is a pointer to 5 integer. p1 is a pointer to whole array of 5 element
    // a is address of 1st element , &a is reference to whole array
    printf("a : %d, &a: %d\n", a, &a);                         // a : 1001313408, &a: 1001313408
    printf("size of a : %d, &a: %d\n", sizeof(a), sizeof(&a)); // size of a : 20, &a: 8
    printf("a+1 : %d, &a+1: %d\n", a + 1, &a + 1);             // a+1 : 1771267908, &a+1: 1771267924
    /**
     * Lets assume int 4 bytes a address start from 100
     *
     * 1 -> 100 - 103, 2 -> 104 - 107, 3 -> 108 - 111, 4 -> 112 - 115, 5 -> 116 - 119
     *
     * a is points to 100
     * if a + 1 is 100 + 1 * 4 -> 104
     *
     * if &a + 1 is 100 + 1 * 4 * 5 -> 120 // here 5 is no.of. element in array
     */

    printf("p1 : %d, *p1 : %d, **p1: %d\n", p1, *p1, **p1); // p1 : 542610816, *p1 : 542610816, **p1 = 1
    // p1 points to 100 - 119
    // *p1 points to 100 - 103
    // **p1 points to 1 (value)

    int a1[][3] = {1, 2, 3, 4, 5, 6};
    int(*ptr)[3] = a1; // a1 is points to 1st row of 3 column
    // ptr points to address on 1 to 3 elements of array
    printf("%d %d\n", (*ptr)[1], (*ptr)[2]); // 2 3
    // *ptr is address of element 1
    // (*ptr)[1] is also written as *((*ptr) + 1) which is 2
    // (*ptr)[2] is also written as *((*ptr) + 2) which is 3
    ++ptr;
    // now ptr points tp address of 4 to 6 elements of array
    printf("%d %d\n", (*ptr)[1], (*ptr)[2]); // 5 6
    // (*ptr)[1] is also written as *((*ptr) + 1) which is 5
    // (*ptr)[2] is also written as *((*ptr) + 2) which is 6

    int j = 27;
    int *jp = &j;
    int **jpp = &jp;
    printf("**jpp : %d, *jp: %d, j : %d\n", **jpp, *jp, j); //**jpp : 27, *jp: 27, j : 27
}