/**
 * Author: E.K.Jithendiran
 */
#include <stdio.h>
void bottomPrymid(int n)
{
    for (int i = 0; i < n; i++)
    {
        for (int j = 0; j < n * 2 - 1; j++)
        {
            if (j >= i && j <= (n * 2 - 2 - i))
                printf("*");
            else
                printf(" ");
        }
        printf("\n");
    }
}

void topPrymid(int n)
{
    for (int i = 0; i < n; i++)
    {
        for (int j = 0; j < 2 * n - 1; j++)
        {
            if (j >= n - i - 1 && j <= n + i - 1)
                printf("*");
            else
                printf(" ");
        }
        printf("\n");
    }
}
void main()
{
    int n;
    printf("Enter number of rows : ");
    scanf("%d", &n);
    topPrymid(n);
    // bottomPrymid(n);
}