/**
 * This program is to demonstrate how stack is allocated for a function
 * compile : gcc -S -masm=intel main.c // to view asm
 * 
 * Ref: https://blog.holbertonschool.com/hack-the-virtual-memory-drawing-the-vm-diagram/
 * https://blog.holbertonschool.com/hack-virtual-memory-stack-registers-assembly-code/
 * https://www.ired.team/miscellaneous-reversing-forensics/windows-kernel-internals/linux-x64-calling-convention-stack-frame
*/
#include<stdio.h>
int main(){
    int a;
    a = 10;
    char c = 'A';
    // float f = 7.14;  ; float is little complex
    printf("Values = %d, %c\n",a,c);
}

/**
 * stack is grow downwords eg: start from 0 to nth address
 * local variables, arguments passed, return address every thing stored in stack that part is called stack frame
 * rbp -> base pointer 
 *          Points to the base address of the current stack frame
 * eg: rbp points to 0
 * 
 * rsp -> stack pointer
 *          Points to the top address of the current stack frame 
 * 
 * eg: rsp points to some n address which will be equal or greater than rbp
 * 
 * when ever a function is called old value of rbp is pushed to stack and 
 * rsp address is moved to rbp to find the base point of the function
 * then move below 10 bytes to store local variables
 * 
 * how looks is rbp is like start context of function and rsp is bottom of function
 * rbp is static until function return
 * function scope from rbp to rsp
 * 
 * arguments are stored above the rbp
 * local variables re stored below the rbp
*/

/**
 * $ gcc main.c
 * 
 * $ gdb ./a.out
 * // place break point at main
 * $ layout asm
 * $ b *main
 * $ run
 * $ info registers rbp rsp
 * // rbp       0x1                 1
 * // rsp       0x7fffffffdcf8      1407737488346360
 * $ stepi // expected to enter into main
*/

// 0x7fffffffdbf8
// 0x7fffffffdbf0