#include <stdio.h>
// function syntax
/**
 * return_type function_name(parameter) {
 * function body..
 *
 * return;
 * }
 */

int areaOfRectangle(int, int); // function declear or function prototype
char test(int n);              // pararameter with variable name is optional

int areaOfSquare(int b)
{
    // this fucntion is defined before calling function so need for function declearation
    return b * b;
}
void main()
{
    printf("Area of square : %d\n", areaOfSquare(4));          // Area of square : 16
    printf("Area of rectangle : %d\n", areaOfRectangle(2, 4)); // Area of rectangle : 8
    // printf("Alphabet: %c\n", alphabet()); conflicting types for 'alphabet'; have 'char()'
    //  why above line is error ?
    /**
     * because there is no function declearation before calling the function so compiler assume default return type is int
     * Compiler will not assume anything about parameter
     * but in the function definition it's retirn type is char so it will produce conflict error
     */
    printf("Number : %d\n", number()); // Number : 2
    functionOverloading();             // Func 1
}

// default return type of a function is int
areaOfRectangle(int l, int b)
{
    // this function is defined after calling function so function declearation is mandatory
    return l * b;
}

number()
{
    return 2;
}

char alphabet()
{
    return 'a';
}

functionOverloading()
{
    printf("Func 1 \n");
    return 2;
}

// error: redefinition of 'functionOverloading'
// functionOverloading(int i)
// {
//     printf("Func 2 \n");
//     return 2;
// }

char test(int i)
{
    return 'a';
}