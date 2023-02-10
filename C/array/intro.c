#include <stdio.h>
#define nn 10
void main()
{
    int n;
    int b[] = {1, 2, 3, 4, 5};
    int c[n];

    int a[n = 5]; // here 5 is assigned to n and n no.of.element placed
    //  for 1 dimensional array must to give no.of.elements or define array values like b
    for (int i = 0; i < n; i++)
    {
        /* code here */
        printf("%d ", a[i]); // default of array values is garbage value
    }
    printf("\n");
    for (int i = 0; i < n; i++)
    {
        printf("Enter value for : %d ", i);
        scanf("%d", &a[i]);
    }

    for (int i = 0; i < n; i++)
    {
        printf("Value of : %d\n", a[i]);
    }

    int d[10] = {1, 2}; // remaining element are filled with 0
    printf("D element \n");
    for (int i = 0; i < nn; i++)
    {
        printf("%d ", d[i]); // 1 2 0 0 0 0 0 0 0 0
    }

    // int e[10] = {1};
    int e[nn] = {};
    printf("\nE element \n");
    for (int i = 0; i < nn; i++)
    {
        printf("%d ", e[i]); // 1 0 0 0 0 0 0 0 0 0
    }

    int f[2] = {1, 2, 3};
    printf("\nF element \n");
    for (int i = 0; i < 3; i++)
    {
        printf("%d ", f[i]); // 1 2 1 last value is garbage value
    }

    int g[nn] = {[0] = 1, [5] = 2, [6] = 3}; // designator initialization
    // int g1[nn] = {[0] = 1, [5] = 2, [6] = 3, [11] = 4};  error: array index in initializer exceeds array bounds
    printf("\ng element \n");
    for (int i = 0; i < nn; i++)
    {
        printf("%d ", g[i]); // 1 0 0 0 0 2 3 0 0 0
    }

    int g2[] = {[0] = 1, [9] = 1}; // length is 10
    printf("\ng2 element \n");
    for (int i = 0; i < sizeof(g2) / sizeof(int); i++)
    {
        printf("%d ", g2[i]); // 1 0 0 0 0 0 0 0 0 1
    }

    int g3[] = {1, 2, 3, [5] = 4}; // 0 -> 1, 1 -> 2, 2 -> 3, 3 -> 0, 4 -> 0, 5 -> 4

    const int a1[2] = {1, 2};
    // a1[1] = 3; error: assignment of read-only location 'a1[1]'
}