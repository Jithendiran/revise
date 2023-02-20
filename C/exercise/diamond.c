/**
 * Author: E.K.Jithendiran
 */
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
            if ((n > i && j >= n - i - 1 && j <= n + i - 1) ||
                (n <= i && j >= (n - (r - i)) && j <= r - ((r - n - (r - i) + 2))))
                printf("*");
            else
                printf(" ");
        }
        printf("\n");
    }
}