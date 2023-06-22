/**
 * Author: E.K.Jithendiran
 */
#include <stdio.h>
void call_by_value(int a, int b)
{
    // here a, b, c variables are created when function is called and destroyed when it exit the function call
    int c = a;
    a = b;
    b = c;
}

void call_by_reference(int *a, int *b)
{
    // here a,b, c variables are created when function is called and destroyed when it exit the function call
    // But a and b are pointers which means it will store address
    // a and b is storing address, to access alue of address use *
    int c = *a;
    *a = *b;
    *b = c;
}
void main()
{
    int a = 5, b = 10;
    // calby
    printf("Bf : A : %d, B : %d\n", a, b);
    call_by_value(a, b);
    printf("Af call_by_value : A : %d, B : %d\n", a, b);

    call_by_reference(&a, &b); // & is address of operator it will pass address of variable as argument
    printf("Af call_by_reference : A : %d, B : %d\n", a, b);
}