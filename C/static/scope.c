#include <stdio.h>
#include <stdlib.h>
/**
 * Segment info will saved in elf file
 * text/code segment readonly (lowest memory address)
 * data segment read/write
 *      initialized data
 *              Global, extern, static, constant global variables
 *      uninitialized data (bss) [set to zero]
 *          uninitilized global, static, extern, constant global variables
 
 * heap segment
 *      dynamic memory allocation
 *      grow up
 
 * stack segment
 *      local variables
 *      function arguments
 *      return address
 *      grow down (start with highest memory address)

 */

 /**
  * Order 
  * ------------
    Text Segment (Code)
    Data Segment (Initialized data)
    BSS Segment (Uninitialized data)
    Heap
    env
    Stack
  */

// auto int x = 30; // auto not allowed to init in global
// auto, register variables store in stack

int globe = 10;     // data init segment 
int uninit;         // data bss segment
const int con = 10; // data init segment
static int stat = 10; // data init segment (file reference only)

extern int etext; // text segment // extern take values from other file
extern int edata; // data segment
extern int end;  // bss segment

extern int def = 34; // not thow error bcz it defined globally. Here extern is going to declare and define
// above extern just throw warning (warning: 'def' initialized and declared 'extern')

int main()
{
    printf("uninit : %d\n", uninit); // uninit : 0
    printf("stat : %d\n", stat); // stat : 10
    printf("stat address %p\n",&stat);
    {
        // this stat is different from the above stat
        // block scope
        static int stat = 1; // data init segment
        printf("stat : %d\n", stat); // stat : 1
        stat = 2;
        printf("stat : %d\n", stat); // stat : 2
        printf("stat address %p\n",&stat);

    }

    printf("stat : %d\n", stat); // stat : 10
    printf("stat address %p\n",&stat);
    
    printf("Global BF modify : %d\n", globe); // Global BF modify : 10
    globe = 11;
    printf("Global AF modify : %d\n", globe); // Global AF modify : 11
    printf("\n------------------------\n");
    
    
    int a = 1, b = 2;// padding
    char cha = 'a'; //;   // stack segment
    float c = 9.7;
    printf("C is float : %f\n", c);           // C is float : 9.700000
    printf("a : %d , b : %d\n", a, b);        // a : 1 , b : 2
    {
        int a = 5;  // stack segment
        printf("a : %d , b : %d\n", a, b); // a : 5 , b : 2
        b = 3;
        printf("B af change : %d\n", b); // B af change : 3
        int c = 6;
        printf("C is int : %d\n", c);                 // C is int : 6
        printf("Global BF modify INN : %d\n", globe); // Global BF modify INN : 11
        globe = 1;
        printf("Global AF modify INN : %d\n", globe); // Global AF modify INN : 1
    }
    
    printf("Global View : %d\n", globe); // Global View : 1
    printf("a : %d , b : %d\n", a, b);   // a : 1 , b : 3

    // heap
    int *ptr = (int *)malloc(4);
    /**
     * in the above line malloc will allocate 4 bytes of memory and return 1st byte address as void pointer
     *  (int *) is used to typecast void pointer to int pointer
     */
    printf("Value of 1st byte : %d\n", *ptr); // Value of 1st byte : 0

    *ptr = 5;
    printf("Value of ptr : %d\n", *ptr); // Value of ptr : 5

    // extern is only for global variables
    // extern we will tell complier to not define
    // extern used when a particular file needs to access variable from another file.
    // when using extern we are going to reuse the varible value that defined some where.
    // Linker's job to get value of ed from other file
    extern int def; //     // extern will tell compiler go out of my scope and find replace the value
    printf("def value : %d\n", def); // J value : 34
    // extern int gg = 90; // error: 'gg' has both 'extern' and initializer

    printf("==========================================\n");

    printf("Order\n");
    printf("------------\n");
    printf("Text Segment (Code)\n");
    printf("Text : %p\n", main); // Text : 0x55f3b3b1b1b0
    printf("end of text %p\n", &etext);
    printf("\n");
    
    printf("Data Segment (Initialized data)\n");
    printf("Data : %p\n", &globe); // Data : 0x55f3b3b1c020
    printf("end of data %p\n", &edata);
    printf("\n");
    
    printf("BSS Segment (Uninitialized data)\n");
    printf("BSS %p\n", &uninit);
    printf("end %p\n", &end);
    printf("\n");
    
    printf("Heap segment\n");
    int *ptr1 = (int *)malloc(10);
    printf("Heap : %p\n", ptr1); // Heap : 0x55f3b3b1c2a0
    ptr1 = (int *)malloc(10);
    printf("Heap : %p\n", ptr1); // It' address must greater that above
    printf("\n");
    
    printf("Stack segment\n");
    printf("Stack : %p\n", &a); 
    printf("Stack : %p\n", &b); // address will low compared to a
    // stack grow down, so start at highest
   
    printf("------------\n");


    return 0;
}

/**
 * 
 * Output

uninit : 0
stat : 10
stat address 0x404034
stat : 1
stat : 2
stat address 0x404038
stat : 10
stat address 0x404034
Global BF modify : 10
Global AF modify : 11

------------------------
C is float : 9.700000
a : 1 , b : 2
a : 5 , b : 2
B af change : 3
C is int : 6
Global BF modify INN : 11
Global AF modify INN : 1
Global View : 1
a : 1 , b : 3
Value of 1st byte : 0
Value of ptr : 5
==========================================
Order
------------
Text Segment (Code)
Text : 0x401156
end of text 0x40152d

Data Segment (Initialized data)
Data : 0x404030
end of data 0x40403c

BSS Segment (Uninitialized data)
BSS 0x404040
end 0x404048

Heap segment
Heap : 0x348b86d0
Heap : 0x348b86f0

Stack segment
Stack : 0x7fffc073c3bc
Stack : 0x7fffc073c3b8
------------


=== Code Execution Successful ===

 */