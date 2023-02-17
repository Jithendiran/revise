// 1st read intro.txt
// next calloc.c
#include <stdio.h>
#include <stdlib.h>
void main()
{
    /**
     * malloc = memory allocation
     * Malloc is a built in function declared in stdlib.h
     *
     * malloc is the short name for memory allocation and is used to dynamically allocate
     * single large block of contiguous memory according to the size specified
     *
     * syntax (void *)malloc (size_t n)
     * Allocate memory based on size specified and return first byte of the allocated memory
     * else return NULL
     *
     * memory allocated in malloc is initilized to garbage value
     */

    int *ptr = (int *)malloc(4);
    /**
     * in the above line malloc will allocate 4 bytes of memory and return 1st byte address as void pointer
     *  (int *) is used to typecast void pointer to int pointer
     */
    printf("Value of 1st byte : %d\n", *ptr); // Value of 1st byte : 0

    *ptr = 5;
    printf("Value of ptr : %d\n", *ptr); // Value of ptr : 5

    // store array element in dynamic memory
    int n;
    printf("Enter number of element : ");
    scanf("%d", &n);
    int *ptr1 = (int *)malloc(n * sizeof(int));

    for (int i = 0; i < n; i++)
    {
        scanf("%d", ptr1 + i); // 1 2 3 4 5
    }
    printf("Entered value \n");
    for (int i = 0; i < n; i++)
    {
        printf("%d ", *(ptr1 + i)); // 1 2 3 4 5
    }
    printf("\n");

    // free
    /**
     * free is used to release the dynamically allocated memory in heap
     * syntax
     *  void free(ptr)
     *  ptr is pointer to the memory
     *
     *  the memory allocated in heap will not be rleased automatically after using the memory.
     *  the space remains there and can't be used
     *  it is programmer's responsibility to release the memory after use
     */

    // berfor release
    printf("BE Address of ptr : %d %p\n", ptr, ptr); // BE Address of ptr : 1431671456 0x5555555592a0
    printf("BE content of ptr : %d \n", *ptr);       // BE content of ptr :  5

    free(ptr);
    // ptr is now become an dangling pointer
    /**
     * dangling pointer is a pointer which is points to some non-existing memory location
     * ptr is still pointing to deallocated memory
     */
    printf("AF Address of ptr : %d %p\n", ptr, ptr); // AF Address of ptr : 1431671456 0x5555555592a0
    printf("AF content of ptr : %d \n", *ptr);       // AF content of ptr : 1431655769
    ptr = NULL;                                      // dangling problem solved
    // here ptr is an NULL pointer
    /**
     * Null pointer is a pointer that does not point to any memory location
     *
     * The value of NULL is 0 but this 0 is not equalent to int 0
     */
    printf("size of NULL : %d\n", sizeof(NULL)); // size of NULL : 8 (depends on machine)
    printf("%d %p\n", ptr, ptr);                 // 0 (nil)
    // printf("%d\n", *ptr); segmentation fault

    free(ptr1);
    ptr1 = NULL;
}