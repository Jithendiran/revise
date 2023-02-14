#include <stdio.h>

int add(int a, int b)
{
    return a + b;
}
void main()
{

    int (*ptr)(int, int) = &add;
    // int (*ptr)(int, int);
    // ptr = &add; // ptr=add;
    printf("%d\n", ptr(1, 2));
}