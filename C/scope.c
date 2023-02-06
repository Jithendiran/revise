#include <stdio.h>
int globe = 10;
void main()
{
    int a = 1, b = 2;
    float c = 9.7;
    printf("Global BF modify : %d\n", globe);// Global BF modify : 10
    globe = 11;
    printf("Global AF modify : %d\n", globe);// Global AF modify : 11
    printf("C is float : %f\n", c);    // C is float : 9.700000
    printf("a : %d , b : %d\n", a, b); // a : 1 , b : 2
    {
        int a = 5;
        printf("a : %d , b : %d\n", a, b); // a : 5 , b : 2
        b = 3;
        printf("B af change : %d\n", b); // B af change : 3
        int c = 6;
        printf("C is int : %f\n", c); // C is int : 9.700000
        printf("Global BF modify INN : %d\n", globe);// Global BF modify INN : 11
        globe = 1;
        printf("Global AF modify INN : %d\n", globe);// Global AF modify INN : 1
    }
    printf("Global View : %d\n", globe); //Global View : 1
    printf("a : %d , b : %d\n", a, b); // a : 1 , b : 3
}