/**
 * Author: E.K.Jithendiran
 */
#include <stdio.h>

/**
 * How to read function pointers
 */
int *(*fun)(int, int, int);
/**
 * fun* indicated fun is a pointer to the function
 * int* indicates it return integer pointer
 * (int, int, int) indicates it takes 3 arguments
 */
int add(int a, int b)
{
    return a + b;
}

int mul(int a, int b)
{
    return a * b;
}

/**
 * outerfun  is a function name
 * 1st argument is function pointer which return int and accepts 2 int as arguments
 * 2nd and 3rd arguments are int
 */
int outerfun(int (*call)(int, int), int x, int y)
{
    printf("In outer function\n");
    return call(x, y);
}

/**
 * select_op() is a function that has 1 int arguments, this will return a function pointer
 * the returned function will return int and accepts 2 int arguments
 */
int (*select_op(int op))(int a, int b)
{
    switch (op)
    {
    case 1:
        return &add;
        break;
    case 2:
        return mul;
        break;
    }
}
void main()
{

    int (*ptr)(int, int) = &add;
    // int (*ptr)(int, int);
    // ptr = &add; // ptr=add;

    printf("%d\n", ptr(1, 2)); // 3

    // printf("%d\n", *ptr(1, 1)); // * will misunderstand for return type
    printf("%d\n", (*ptr)(1, 1)); // 2 // this will work

    printf("%d\n", outerfun(add, 1, 2));
    /**
     * In outer function
       3
    */
    printf("%d\n", outerfun(mul, 5, 2));
    /**
     * In outer function
       10
    */

    // array of function pointers
    int (*arr_fun[])(int, int) = {add, mul};
    printf("array fun pointers = %d\n", arr_fun[0](1, 9));    // array fun pointers = 10
    printf("array fun pointers = %d\n", (*arr_fun[1])(1, 9)); // array fun pointers = 9

    // function return function pointer
    int (*op)(int, int) = select_op(1);
    printf("Returned function pointer = %d \n", op(1, 1)); // Returned function pointer = 2

    printf("call returned function pointer = %d \n", select_op(2)(2, 3)); // call returned function pointer = 6

    // function pointer airthmetic
    // ptr++; // It will crash the program
    // printf("pointer %d\n", ptr(1, 2));
    for (int i = 0; i < 2; i++)
    {
        printf("Pointer airth %d\n", (*(arr_fun + i))(2, 9));
    }
    /**
     * Op
        Pointer airth 11
        Pointer airth 8
    */
}