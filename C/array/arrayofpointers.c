/**
 * Author: E.K.Jithendiran
 */
#include <stdio.h>
void main()
{
    // array of pointers
    // It stores address of memory in each array element
    int a[] = {1, 2, 3};
    //    int *ptr[3] = &a; // it is not array of pointers

    int *ptr[] = {&a[0], &a[1], &a[2]}; // array of pointers to int
    // Each element in ptr, holds a pointer to an int value
    // ptr is array which store poniters
    for (int i = 0; i < 3; i++)
    {
        printf("ptr = %d, *ptr = %d, value = %d\n", ptr + i, *(ptr + i), *(*(ptr + i)));
    }
    /**
     * Op
        ptr = 551744272, *ptr = 551744308, value = 1
        ptr = 551744280, *ptr = 551744312, value = 2
        ptr = 551744288, *ptr = 551744316, value = 3
    */
   /**
    * a
    * |---------|---------|--------|
    * |  1      |    2    |    3   |
    * |---------|---------|--------|           
    *   100      104        108
    * 
    * ptr
    * |---------|---------|---------|
    * | 100     | 104     | 108     |
    * |---------|---------|---------|
    *  200        204       208
    * 
    *   ptr points to 200
    *   *ptr ponits to 100
    *   **ptr ponits to 1
   */   

 // poniter to an array of 3 ints
  int (*ptr1) [3] = {&a[0], &a[1], &a[2]}; // error
    // int (*ptr1)[3] = {a[0], a[1], a[2]};
  printf("Size of  *ptr = %d, *ptr1 = %d\n",sizeof(ptr),sizeof(*ptr1)); // Size of  *ptr = 24, *ptr1 = 12
 for (int i = 0; i < 3; i++)
    {
        printf("ptr = %d, *ptr = %d, value = %d\n", ptr1 + i, *(ptr1 + i), *(*(ptr1 + i)));
    }
    /**
     * Op
        ptr = 551744308, *ptr = 551744308, value = 1
        ptr = 551744320, *ptr = 551744320, value = 551744308
        ptr = 551744332, *ptr = 551744332, value = 3
    */
  // https://www.w3resource.com/c-programming/c-arrays-and-pointers.php
}