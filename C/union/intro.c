/**
 * Author: E.K.Jithendiran
 */
#include <stdio.h>
/**
 * Union are sharing same memory location
 * size of the union is taken according to the largest member of the union
 */
union a
{
    int a;
    char b;
    int c;
};

void main()
{
    union a var, *ptr;
    var.a = 65;
    printf("int : %d, char : %c\n", var.a, var.b);                        // int : 65, char : A
    printf("Size of Union : %d\n", sizeof(var));                          // Size of Union : 4
    printf("Address of var = %p, a = %p,b = %p\n", &var, &var.a, &var.b); // Address of var = 0x7fffffffdc9c, a = 0x7fffffffdc9c,b = 0x7fffffffdc9c

    /**
     * assume var start address is 1000
     *
     * int has larger size that char so union's int size (4 byte)
     * a,b,c has start address of 1000 and b is end in 1001 but a,c address end in 1004
     */
    // pointer access are same as structure
    ptr = &var;
    printf("Pointer access of c = %d\n", ptr->c); // Pointer access of c = 65
}