#include <stdio.h>
int a = 5; // it is stored in initilized data segment in memory

int fun1(int);
int fun2(int);

// start doubt
static int localfun()
{
    int a;
    printf("A : %d\n", a);
}

static int localfun1()
{
    int as;
    printf("AS : %d\n", as);
}
// end doubt

void main()
{
    int a = 5;             // it is local variable to main function
    a = fun1(a);           // 20
    printf("a : %d\n", a); // a : 20
    localfun();            // A : 32733
    localfun1();           // AS : 32733
}

int fun2(int b)
{
    // local variables c,b
    // a is retrived from global variable
    int c;
    c = a + b; // 5 + 15 -> 20
    return c;  // 20
}

int fun1(int b)
{
    // b is local variable
    b = b + 10;  // 5 + 10 -> 15
    b = fun2(b); // 20
    return b;    // 20
}