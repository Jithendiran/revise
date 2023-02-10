#include <stdio.h>

int *getFist(int a[], int n)
{
    printf("size of a func : %d\n", sizeof(a)); // size of a func : 8
    // In this function a is base address of passed array
    for (int i = 0; i < n; i++)
    {
        printf("%d ", *(a + i)); // printf("%d ", a[i]);
    }
    printf("\n");
    return &a[0]; // retrun a; // both are same bcz both are reference to 1st address
}

void main()
{
    // Array are based on pointers
    int a[] = {12, 22, 32};
    int *p = getFist(a, 3);                                                                                                                      // a is base address of array a
    printf("Size of char pointer : %d, size of int pointer : %d, size of float pointer : %d\n", sizeof(char *), sizeof(int *), sizeof(float *)); // Size of char pointer : 8, size of int pointer : 8, size of float pointer : 8
    printf("%d return value\n", *p);                                                                                                             // 12 return value
    printf("size of a main : %d\n", sizeof(a));                                                                                                  // size of a main : 12
}