/*
* Author: E.K.Jithendiran
* Date: 17 Sep 2025
*/
// echo 0 | sudo tee /proc/sys/kernel/randomize_va_space // use this to disable ALSR
// gcc -g -O0 stack.c -o /tmp/pgm.out

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
void func() {

/*
How stack grow, how stack variable ia layouted inside stack
stack grow downward high to low
x address is higher than arr
eg: x address is 20, 19, 18, 17
arr starts from 16, 15, 14, 13, 12, 11, 10, 9, 8, 7 
arr[0] pooint to 7 and arr10 point to 16
| Address | Contents   |
|---------|------------|
|   20    | x (byte 3) |
|   19    | x (byte 2) |
|   18    | x (byte 1) |
|   17    | x (byte 0) |
|   16    | arr[9]     |
|   15    | arr[8]     |
|   14    | arr[7]     |
|   13    | arr[6]     |
|   12    | arr[5]     |
|   11    | arr[4]     |
|   10    | arr[3]     |
|   9     | arr[2]     |
|   8     | arr[1]     |
|   7     | arr[0]     |

These are throriatically correct but compiler may arrange x at lower address and arr in higher address
Stack will grow downwords, func() address will be lower than main, but the layout of variable's inside function is depend compiler
compiler may arrange x then arr or arr then x
verify this in `objdump -d -M intel -S /tmp/pgm`
*/

    int x = 10;     
    char arr[10] = "hi";
    printf("func stack addr: %p\n", (void *)&x);

}

/*
$ objdump -d -M intel -S /tmp/pgm
// RSP stands for Register Stack Pointer, The stack pointer keeps track of the top of the stack in memory.
// RBP stands for Register Base Pointer, It’s used to point to the base of the current stack frame during function execution.

dis-asm of func, every thing in hexa deximal for example 0x20 is 2 * 16 + 0 = 36.
0000000000001169 <func>:
    1169:       f3 0f 1e fa             endbr64                             // don't focus here, don't want this
    116d:       55                      push   rbp                          // save old base pointer, on the stack
    116e:       48 89 e5                mov    rbp,rsp                      // This sets the current base pointer (rbp) to the current top of the stack (rsp). It marks the start of this function's stack frame. Using rbp we can access local viriable by offset
    1171:       48 83 ec 20             sub    rsp,0x20                     // This reserves 0x20 = 36 bytes of stack space for local variables, by moving down so sub, why 36?  compiler add padding because it aligns to multiple of 16
    1175:       64 48 8b 04 25 28 00    mov    rax,QWORD PTR fs:0x28        // some safety instruction, don't focus
    117c:       00 00 
    117e:       48 89 45 f8             mov    QWORD PTR [rbp-0x8],rax       // Used to protect the stack from overflow attacks (stack smashing protector).
    1182:       31 c0                   xor    eax,eax                      // clearing the eax, it is often used as return value
    1184:       c7 45 e8 0a 00 00 00    mov    DWORD PTR [rbp-0x18],0xa     // int x = 10;  a-> 10
    118b:       48 c7 45 ee 68 69 00    mov    QWORD PTR [rbp-0x12],0x6968  // char arr[10] = "hi"; -> 69 -> h 68 -> i
    1192:       00 
    1193:       66 c7 45 f6 00 00       mov    WORD PTR [rbp-0xa],0x0       // store null at end of 0x6968
    1199:       48 8d 45 e8             lea    rax,[rbp-0x18]
    119d:       48 89 c6                mov    rsi,rax
    11a0:       48 8d 05 5d 0e 00 00    lea    rax,[rip+0xe5d]        # 2004 <_IO_stdin_used+0x4>
    11a7:       48 89 c7                mov    rdi,rax
    11aa:       b8 00 00 00 00          mov    eax,0x0
    11af:       e8 bc fe ff ff          call   1070 <printf@plt>
    11b4:       90                      nop
    11b5:       48 8b 45 f8             mov    rax,QWORD PTR [rbp-0x8]
    11b9:       64 48 2b 04 25 28 00    sub    rax,QWORD PTR fs:0x28
    11c0:       00 00 
    11c2:       74 05                   je     11c9 <func+0x60>
    11c4:       e8 97 fe ff ff          call   1060 <__stack_chk_fail@plt>
    11c9:       c9                      leave
    11ca:       c3                      ret


//-----------------variable layout-----------
    1184:       c7 45 e8 0a 00 00 00    mov    DWORD PTR [rbp-0x18],0xa     // int x = 10;  a-> 10
    118b:       48 c7 45 ee 68 69 00    mov    QWORD PTR [rbp-0x12],0x6968  // char arr[10] = "hi"; -> 69 -> h 68 -> i
here x = 10 is stored at lower address compared to arr
Stack grow downwards but the inside stack variabled are stored in order of what we defined in code low to high


Higher memory addresses (closer to rbp)
|-------------------|
|       rbp         |
|-------------------|
|      ...          |
|-------------------|
| arr[10] at        |
| rbp-0x12          |
|-------------------|
|       ...         |
|-------------------|
| x at rbp-0x18     |
|-------------------|
Lower memory addresses

//---------------------------------------------

*/


int main(){
    // stack
    int x = 4; // 4 is stored in stack
    char *y="hi"; // Pointer stored in Stack, string literal stored in Read-Only Data
    printf("main stack addr: %p\n", (void *)&x);
    func();
}

/*
main stack addr: 0x7fffffffdc2c
func stack addr: 0x7fffffffdbf8

in main and func 0x7fffffffd is common, so remaining us 
main -> c2c
func -> bf8

c is greater than b, now we confirmed stack grow downwards
when new function call is happened it will push the stack frame at lower address, that's why func address is lesser
*/