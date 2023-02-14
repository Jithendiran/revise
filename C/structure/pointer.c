#include <stdio.h>

struct a
{
    int x;
};

struct n
{
    int x;
    int *ptr;
};
void main()
{
    struct a a1, *p;
    a1.x = 2;
    p = &a1;
    printf("%d : %d\n", p->x, (*p).x); // 2 : 2

    struct n n1, *p1, *p2;
    n1.x = 27;
    n1.ptr = &n1.x;
    printf("x : %d, ptr : %d\n", n1.x, *n1.ptr); // x : 27, ptr : 27

    p1 = &n1;
    printf("x : %d, ptr: %d\n", p1->x, *p1->ptr); // x : 27, ptr: 27
}