/**
 * Author: E.K.Jithendiran
 */
// gcc static.c static2.c
#include <stdio.h>

// start doubt
static int localfun()
{
    int a; // these values are store in data segment? if so it need to be init with 0 only but it gives 3 why?
    printf("A : %d\n", a);
}

static int localfun1()
{
    int as;
    printf("AS : %d\n", as);
}
// end doubt

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

    // static function call
    // statFunction(); Linker error  undefined reference to `statFunction'
    localfun();  // A : 3 why 3 ?
    localfun1(); // AS : 3
    // why both has same value
    /**
     * the same value because both functions have same amout of data in stack
     * ref: https://blog.holbertonschool.com/hack-virtual-memory-stack-registers-assembly-code/
    */
    // see another file scope_of_var.c
}