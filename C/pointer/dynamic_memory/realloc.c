// 1st read calloc.c
#include <stdio.h>
#include <stdlib.h>

void print_array(int *ptr, int n)
{
    printf("Entered array\n");
    for (int i = 0; i < n; i++)
    {
        printf("%d ", *(ptr + i)); // 1 2 3 4 5
    }
    printf("\n");
}
void main()
{
    /**
     * reallos is reallocation
     * realloc is used to change the size of the memory block without losing old data
     *
     * Syntax
     * void *realloc(void *ptr, size_t newsize)
     * *ptr is pointer to previously allocated memory
     * newsize is new size of the memory
     *
     * on failure realloc will return NUll
     *
     * realloc will allocate new memory but content of old block is moved to new block automatically
     * may lose the data when the new size is smaller than old size
     * newely allocated bytes are uninitialized
     */

    int *ptr;
    // here ptr is wild pointer
    /**
     * wild pointer is also known as uninitialized pointers
     *
     * these pointers usually points to some arbitrary memory location and may cause a program to crash or misbehave
     */
    // ptr = (int *)realloc(ptr, 4*sizeof(int)); // this may leads to misbehave
    ptr = (int *)malloc(2 * sizeof(int));
    printf("Enter two int value\n");
    scanf("%d %d", ptr, (ptr + 1)); // 1 2
    print_array(ptr, 2);

    // now 2 bytes is not sufficient so another 2 block of memory is needed with old data
    ptr = (int *)realloc(ptr, 4 * sizeof(int));
    printf("Enter two int value\n");
    scanf("%d %d", (ptr + 2), (ptr + 3)); // 1 2 3 4
    print_array(ptr, 4);

    ptr = (int *)realloc(ptr, 2 * sizeof(int));
    print_array(ptr, 4); // 1 2 3 4 (3,4 may affect)

    free(ptr);
    ptr = NULL;
}