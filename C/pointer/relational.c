#include <stdio.h>
void main()
{
    int a[] = {1, 2, 3, 4};
    int *p = &a[1], *p1 = &a[1];
    int *q = &a[2];
    printf("P > q : %d\n", p > q);     // P > q : 0 (p address is before q)
    printf("P < q : %d\n", p < q);     // P < q : 1 (p address is after q)
    printf("P == p1 : %d\n", p == p1); // P == p1 : 1 (both are same address)
    int c = 2;
    int *p2 = &c;
    printf("P > p2 : %d\n", p > p2); // P > p2 : 1 (answer may varies because it is based on memory allocation)
}