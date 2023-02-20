/**
 * Author: E.K.Jithendiran
 */
#include <stdio.h>
// enumeration in c is also an user defined data type
enum Bool
{
    False,
    True
}; // if value not specified it start with 0
enum Test
{
    a = 1,
    b
};

enum Test1 // two or mor enum can share same value
{
    a1 = 3, // if a and b name is used  (redeclaration of enumerator ‘a’ error will be shown)
    b1 = 3,
    c, // c will get value 4 values is calculted based up on previous + 1
    d = 2,
    // e = 3.5 only int values are allowed
};
void main()
{
    enum Bool var;
    var = False;
    printf("Var : %d\n", var); // Var : 0
    var = True;
    printf("Var : %d\n", var); // Var : 1
    var = 0;
    printf("Var : %d\n", var); // Var : 0
    var = 1;
    printf("Var : %d\n", var); // Var : 1

    var = 3;
    printf("Var : %d\n", var); // Var : 3

    enum Test var1;
    var1 = a;
    printf("Var1 : %d\n", var1); // Var1 : 1
    var1 = b;
    printf("Var1 : %d\n", var1); // Var1 : 2

    enum Test1 var2;
    var2 = a1;
    printf("Var2 : %d\n", var2); // Var2 : 3
    var2 = b1;
    printf("Var2 : %d\n", var2); // Var2 : 3
    var2 = c;
    printf("Var2 : %d\n", var2); // Var2 : 4

    var1 = c;
    printf("Var1 : %d\n", var1); // Var1: 4

    int a = True;
    printf("A = %d\n", a); // A = 1
}