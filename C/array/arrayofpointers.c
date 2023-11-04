/**
 * Author: E.K.Jithendiran
 */
#include <stdio.h>
void main()
{
  // [] has more Precedence than * dereference/pointer

  // array of pointers
  // It stores address of memory in each array element
  int a[] = {1, 2, 3};
  //    int *ptr[3] = &a; // it is not array of pointers

  int *ptr[] = {&a[0], &a[1], &a[2]}; // array of pointers to int
  // here 1st ptr[] is parsed then int * is parsed so it is array of pointers
  // Each element in ptr, holds a pointer to an int value
  // ptr is array which store poniters
  for (int i = 0; i < 3; i++)
  {
    printf("ptr = %d, *ptr = %d, value = %d\n", ptr + i, *(ptr + i), *(*(ptr + i)));
  }
  /**
   * Op
      ptr = 1209211456, *ptr = 1209211492, value = 1
      ptr = 1209211464, *ptr = 1209211496, value = 2
      ptr = 1209211472, *ptr = 1209211500, value = 3
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
  int(*ptr1)[3] = {&a[0], &a[1], &a[2]};
  // *ptr1 is parsed first then [3] so it is pointer to array
  // int (*ptr1)[3] = {a[0], a[1], a[2]}; // error
  printf("Size of ptr = %d, ptr1 = %d, *ptr1 = %d\n",sizeof(ptr) , sizeof(ptr1), sizeof(*ptr1)); // Size of ptr = 24, ptr1 = 8, *ptr1 = 12

  // size of ptr is 24 because it store 3 pointer
  // size of ptr1 is 8, it is a single pointer to array
  // size of *ptr is 12, it pointer to 1D array of 3 int
  for (int i = 0; i < 3; i++)
  {
    printf("ptr = %d, *ptr = %d, value = %d\n", ptr1 + i, (*ptr1) + i, *((*ptr1) + i));
  }
  /**
   * Op
      ptr = 1209211492, *ptr = 1209211492, value = 1
      ptr = 1209211504, *ptr = 1209211496, value = 2
      ptr = 1209211516, *ptr = 1209211500, value = 3
  */

  int(*ptr2)[3] = &a;
    // It is pointer to songle array which contains 3 element
  printf("size of *ptr2 = %d\n", sizeof(ptr2));      // size of *ptr2 = 8
  printf("ptr2 = %d, ptr+1 = %d\n", ptr2, ptr2 + 1); // ptr2 = 1209211492, ptr2+1 = 1209211504
                                                     // 1209211504 - 1209211492 = 12
                                                     // size of ptr2 is 8 bytes but ptr2+1 add 12 bytes
                                                     // because (*ptr2)[3] is pointer to array of 3 ints

  for (int i = 0; i < 3; i++)
  {
    printf("address = %d, value = %d\n", (*ptr2) + i, (*(*ptr2) + i));
  }

  /**
   * Op
      address = 1209211492, value = 1
      address = 1209211496, value = 2
      address = 1209211500, value = 3
  */

   /**
   * a
   * |---------|---------|--------|
   * |  1      |    2    |    3   |
   * |---------|---------|--------|
   *   100      104        108
   *
   * ptr2
   * -----------------------------------
   * | |---------|---------|---------| |
   * | | 1       | 2       | 3       | |
   * | |---------|---------|---------| |
   * | 200        204       208        |
   * -----------------------------------
   * 200                                212
   *   ptr2 points to 200
   *   *ptr ponits to 200
   *   **ptr ponits to 1
   * 
   *   ptr2 + 1 it will goes to 212
   *   (*ptr2) + 1 it will goes to 204
   */

    printf("size of ptr2 = %d, size of *ptr2 = %d, size of **ptr2 = %d\n", sizeof(ptr2), sizeof(*ptr2), sizeof(**ptr2));
    // size of ptr2 = 8, size of *ptr2 = 12, size of **ptr2 = 4

    printf("ptr2 = %d, *ptr2 = %d, **ptr2 = %d\n", (ptr2), (*ptr2), (**ptr2));
    // ptr2 = 1209211492, *ptr2 = 1209211492, **ptr2 = 1

    // int **ptr3 = ptr2;// error
    int **ptr3 = &ptr2;
    printf("size = %d, %d value= %d\n", sizeof(ptr3),sizeof(&ptr2),**ptr3); // size = 8,8 value= 1
}