#include <stdio.h>
#define PI 3.14159 // using # define
// ; should not be use
// It is the Job of preprocessor to replace Name with value
// PI is Name, 3.14159 is value
// perprocessor can't replace name inside double quotes
#define add(x, y) x + y
#define greater(x, y)                 \
    if (x > y)                        \
        printf("%d is greater\n", x); \
    else                              \
        printf("%d is greater\n", y);
// note here ; is used
void main()
{
    printf("PI : %f\n", PI); // PI : 3.141590 (Preprocessor will replace the value)
    // PI = 1; error: lvalue required as left operand of assignment
    printf("Add : %d\n", add(3, 4)); // Add : 7
    greater(4, 5);
    greater(6, 5);

    const int a = 3; // using const
    // a=3; error
}