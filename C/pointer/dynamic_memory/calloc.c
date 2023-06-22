/**
 * Author: E.K.Jithendiran
 */
// first read malloc.c
// nxt realloc.c
#include <stdio.h>
#include <stdlib.h>
void main()
{
    /**
     * calloc is clear allocation
     * calloc is used to dynamically allocate multiple blocks of memory
     * memory allocated by calloc is initilized to 0
     *
     * Syntax:
     *  (void *)calloc (size_t n, size_t size)
     *  here n is the number of blocks
     *  size is the size of each block
     *
     * on failure calloc will return NUll
     */

    int *ptr = (int *)calloc(10, sizeof(int));
    // equaluent malloc is (int *)malloc(10 * sizeof(int))

    // store array element in dynamic memory
    int n;
    printf("Enter number of element : ");
    scanf("%d", &n);

    for (int i = 0; i < n; i++)
    {
        scanf("%d", ptr + i); // 1 2 3 4 5
    }
    printf("Entered value \n");
    for (int i = 0; i < n; i++)
    {
        printf("%d ", *(ptr + i)); // 1 2 3 4 5
    }
    printf("\n");

    free(ptr);
    ptr = NULL;
}