/**
 * Author: E.K.Jithendiran
 */
// compile gcc var_modifier.c exten.c
// run ./a.out
#include <stdio.h>

/**
 * It is Storage class
 * It determine the default value, where value need to store, scope, life span
 * Types of storage class are auto, register, extern, static
 * default storage class is auto
*/

// auto int x = 30; // auto not allowed to init in global
// auto, register variables store in stack
// register int i = 0;
int globe; // initlize with 0
int j = 27;
extern int def = 34; // not thow error bcz it defined globally. Here extern is going to declare and define
// above extern just throw warning (warning: 'def' initialized and declared 'extern')
// extern static int stat_ex = 345; // error: multiple storage classes in declaration specifiers
static int stat_ex = 345;
void main()
{
    extern int stat_ex; // take value from global
    printf("global static, here extern : %d\n",stat_ex); //global static, here extern : 345
    int inmain;
    printf("inmain value : %d\n", inmain); // some garbage value
    printf("globe value : %d\n", globe);   // 0
    // auto is declare and define
    // auto it automatically destroy when it's scope is gone
    // auto will assign 0 or garbage value by the compiler
    {
        int a = 1;
        auto int b = 2; // both are same and it destroy when the control exit the scope
    }
    // extern is only for global variables
    // extern we will tell complier to not define
    // extern used when a particular file needs to access variable from another file.
    // when using extern we are going to reuse the varible value that defined some where.
    // Linker's job to get value of ed from other file
    // printf("Extern value : %d\n", ed); // error
    extern int ed; // it will get value from extern.c file
    extern int ed; // multiple declare not going to show any error
    extern int ed;
    ed = 7; // it will change value of ed that defined in extern.c
    // extern is just declare not define
    printf("Extern value : %d\n", ed); // Extern value : 7 (if any one file does not contain ed it will throw error)
    // extern int ed1; // Multiple  definition of `ed1'; error when compile
    // d1 has multiple definition in extern.c and extern2.c so it throw error

    // extern will tell compiler go out of my scope and find replace the value
    extern int j;
    printf("J value : %d\n", j); // J value : 27

    // The above example extern is not defined just initlized
    // extern int def = 34;//error: 'def' has both 'extern' and initializer (error if define inside main function)
    printf("The def : %d\n", def); // The def : 34

    // int gg=0;
    // extern int gg; // error

    // extern int gg = 90; // error: 'gg' has both 'extern' and initializer

    // register
    // register keyword hints the compiler to store a variable in a register memory
    // register  memory is very limited but it is very fast to access
    // This is the choice of compiler whether it puts the given variable in register or not
    // Compiler even automatically made necessary optmization

    // default value is garbage, scope is block
    // global declaration not allowed
    // life time is end of the block
    // locatoin cpu register
    register int i = 0;


    int abc = 5;
    register int  ji = 3;
    // int *ptr = &ji; //error address of register variable 'ji' requested
    register int *ptr = &abc;  // this is ok
}