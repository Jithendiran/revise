#include <stdio.h>
void main()
{
    int n;
    printf("Enter number of rows : ");
    scanf("%d", &n);
    int r = n * 2 - 1;
    for (int i = 0; i < r; i++)
    {
        for (int j = 0; j < r; j++)
        {
            if (n > i)
            {
                if (j >= n - i - 1 && j <= n + i - 1)
                    printf("*");
                else
                    printf(" ");
            }
            else
            {
                // printf("i : %d, j : %d , r : %d, n : %d\n", i, j, r - 1, n);
                // if (j >= (r - 1 - i) && j <= ((r - 1) - (r - 1 - i)))
                //     printf("*");
                // else
                //     printf(" ");
                printf("i = %d, start = %d to %d\n", i, r - 1 - i);
                break;
            }
        }
        printf("\n");
    }
}