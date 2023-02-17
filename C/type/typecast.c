#include <stdio.h>
void main()
{
    float f = 4.5;
    double d = 4589.64322;
    int i = 5;
    // float
    printf("int of float = %d\n", (int)f);        // int of float = 4
    printf("double of float = %lf\n", (double)f); // double of float = 4.500000

    // double
    printf("int of double = %d\n", (int)d);     // int of double = 4589
    printf("float of double = %f\n", (float)d); // float of double = 4589.643066

    // int
    printf("float of int = %f\n", (float)i);   // float of int = 5.000000
    printf("double of int = %f\n", (double)i); // double of int = 5.000000

    /**
     * Datatype hierarchie top is higher and bottom is lower
     *
     * long double
     * double
     * float
     * long
     * int
     * short
     * char
     *
     *
     * lower data type can be promoted to higher datatype
     *
     * in some case higher datatype can converted to lower
     * for eg
     */
    int eg = 2.17 + 5;
    // here compiler will treat 5 also to double because 2.17 is double
    // 2.17 + 5.0 = 7.17
    // but assignment operator is int so only 7 is stored in eg remaining .17 is truncated
    printf("Eg as int : %d\n", eg); // Eg as int : 7

    double eg1 = 2.17 + 5;
    printf("Eg as double : %lf\n", eg1); // Eg as double : 7.170000

    int eg2 = 4.67;
    printf("Eg2 as int %d\n", eg2); // Eg2 as int 4

    // all the above type conversion is auto type conversion also called as implict type conversion

    // below are explicit type conversion
    double eg3 = (int)2.17 + 5; // here double is converted to int
    printf("Eg3 : %lf\n", eg3); // Eg3 : 7.000000
}