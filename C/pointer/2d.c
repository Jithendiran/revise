#include <stdio.h>
void main()
{
    int a[][3] = {{1, 2, 3}, {4, 5, 6}};
    int *p = a;
    for (int i = 0; i < 2; i++)
    {
        for (int j = 0; j < 3; j++)
        {
            printf("%d ", *(p + i + j));
        }
        printf("\n");
    }
    /**
     * Op
       1 2 3
       2 3 4
    */

    for (int i = 0; i < 6; i++)
    {
        printf("%d ", *(p + i)); // 1 2 3 4 5 6
    }
    printf("\n");

    // memory representation of 2d array
    // let's assume start address of a is 1000 and int is 4 byte
    // 1 -> 1000 - 1004, 2-> 1005 - 1008, 3 -> 1009 - 1012
    // 4 -> 1013 - 1016, 5 -> 1017 -> 1020, 6 -> 1021 ->1024
    // values are stored in sequentical order
}