// gcc static.c static2.c
#include <stdio.h>

extern int g1;
void main()
{
    int a;
    a = increment();
    a = increment();
    a = increment();
    printf("A is : %d\n", a); // A is : 3

    int b;
    b = incrementStatic();
    b = incrementStatic();
    b = incrementStatic();
    printf("B is : %d\n", b); // B is : 11

    // g1++;warning: relocation against `g1' in read-only (Linker error)

    int c;
    c = incstc();
    c = incstc();
    c = incstc();
    printf("C is : %d\n", c); // C is : 3

    int d;
    d = consins();
    d = consins();
    d = consins();
    printf("D is : %d\n", d); // D is : 6
}