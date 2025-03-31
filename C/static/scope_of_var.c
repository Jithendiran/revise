/**
 * Author: E.K.Jithendiran
 */
// gcc scope_of_var.c
#include <stdio.h>
int a = 5; // it is stored in initilized data segment in memory

int fun1(int);
int fun2(int);

// also see static.c
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

void main()
{
    int a = 5;             // it is local variable to main function
    a = fun1(a);           // 20
    printf("a : %d\n", a); // a : 20
    localfun();            // A : 32733 (garbage)
    localfun1();           // AS : 32733 (garbage)
    // why both have same garbage value is because of stack work
    // both function have same structure and same int variable
    // when localfunc() is loaded in stack, it will allocate a memory for 'a' suppose memory will be address will be 10001 and function is completed, stack pop out
    // now localfun1() is loaded in stack, it also have int, since before function also have int and that function is poped out and now this function will ocupy that space
    // so this function 'as' also have same value as a
    // both has same value
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