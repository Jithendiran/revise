#include <stdio.h>

// void ab(int aa, int bb)
// {
//     int c;
//     printf("In fun aa = %d, bb = %d, c = %d\n", aa, bb, c);
// }
// void main()
// {
//     int a;
//     char c;
//     float f;
//     printf("Int a =%d\n", a);
//     printf("char c =%d\n", c);
//     printf("float f= %d\n", f);
//     ab(1, 2);
// }

/**
 * Op
 *
    Int a =32627
    char c =60
    float f= -40746336
    In fun aa = 1, bb = 2, c = 791621423
*/

// void main()
// {
//     int a, b;
//     printf("Int a =%d, b = %d\n", a, b);
// }

// op
//  Int a =32687, b = 1246231216

// void ab()
// {
//     int a;
//     printf("IN Int a =%d\n", a);
// }

// void main()
// {
//     int a;
//     printf("Int a =%d\n", a);
//     ab();
// }

/**
 * Why this gives always same garbage value?
 * Op
 * Int a =32725
   IN Int a =32725
*/

// void ab()
// {
//     int b;
//     printf("IN Int b =%d\n", b);
// }

// void main()
// {
//     int a;
//     printf("Int a =%d\n", a);
//     ab();
// }

/**
 * op
Int a =32554
IN Int b =32554
*/

void ab()
{
    int b, c;
    printf("IN Int b =%d, c = %d\n", b, c);
}

void main()
{
    int a;
    printf("Int a =%d\n", a);
    ab();
}

/**
 * Op
Int a =32676
IN Int b =32676, c = -884251952
*/