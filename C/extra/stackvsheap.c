/*
* Author: E.K.Jithendiran
* Date: 17 Sep 2025
*/
// echo 0 | sudo tee /proc/sys/kernel/randomize_va_space // use this to disable ALSR
// gcc -g -O0 stackvsheap.c -o /tmp/pgm.out
/*
How data is stored in stack and heap
How it is accessed
Visualize with address, if impossible in address check the asm, have to prove all
*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
void func() {

/*
How stack grow
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

    char *p = malloc(10);
    strcpy(p, "hi");

/*
    (Stack)
    |------------|
    |     p      | -> points to heap addr
    |------------|

    (Heap)
    |------------| <- malloc returns this addr
    |  'h'       | <- p[0] low
    |  'i'       | <- p[1]
    |  '\0'      | <- p[2] high
    |------------|

*/

strcat(p, " man"); // how this will work?
}

/*
dis-asm of func, every thing in hexa deximal for example 0x30 is 3 * 16 + 0 = 48.
00000000000011a9 <func>:
    11a9:	f3 0f 1e fa          	endbr64
    11ad:	55                   	push   rbp
    11ae:	48 89 e5             	mov    rbp,rsp
    11b1:	48 83 ec 30          	sub    rsp,0x30     // This reserves 0x30 = 48 bytes of stack space for local variables.
    11b5:	64 48 8b 04 25 28 00 	mov    rax,QWORD PTR fs:0x28
    11bc:	00 00 
    11be:	48 89 45 f8          	mov    QWORD PTR [rbp-0x8],rax
    11c2:	31 c0                	xor    eax,eax
    11c4:	c7 45 dc 0a 00 00 00 	mov    DWORD PTR [rbp-0x24],0xa     // int x = 10;  a-> 10
    11cb:	48 c7 45 ee 68 69 00 	mov    QWORD PTR [rbp-0x12],0x6968  // char arr[10] = "hi"; -> 69 -> h 68 -> i
    11d2:	00 
    11d3:	66 c7 45 f6 00 00    	mov    WORD PTR [rbp-0xa],0x0
    11d9:	bf 0a 00 00 00       	mov    edi,0xa
    11de:	e8 cd fe ff ff       	call   10b0 <malloc@plt>
    11e3:	48 89 45 e0          	mov    QWORD PTR [rbp-0x20],rax
    11e7:	48 8b 45 e0          	mov    rax,QWORD PTR [rbp-0x20]
    11eb:	66 c7 00 68 69       	mov    WORD PTR [rax],0x6968
    11f0:	c6 40 02 00          	mov    BYTE PTR [rax+0x2],0x0
    11f4:	48 8b 45 e0          	mov    rax,QWORD PTR [rbp-0x20]
    11f8:	48 89 c7             	mov    rdi,rax
    11fb:	e8 80 fe ff ff       	call   1080 <strlen@plt>
    1200:	48 89 c2             	mov    rdx,rax
    1203:	48 8b 45 e0          	mov    rax,QWORD PTR [rbp-0x20]
    1207:	48 01 d0             	add    rax,rdx
    120a:	c7 00 20 6d 61 6e    	mov    DWORD PTR [rax],0x6e616d20
    1210:	c6 40 04 00          	mov    BYTE PTR [rax+0x4],0x0
    1214:	90                   	nop
    1215:	48 8b 45 f8          	mov    rax,QWORD PTR [rbp-0x8]
    1219:	64 48 2b 04 25 28 00 	sub    rax,QWORD PTR fs:0x28
    1220:	00 00 
    1222:	74 05                	je     1229 <func+0x80>
    1224:	e8 67 fe ff ff       	call   1090 <__stack_chk_fail@plt>
    1229:	c9                   	leave
    122a:	c3                   	ret

*/


/*
Memory
High Address
+---------------------+ <- Stack Top (main, then func frames)
|    Local vars       |
|    arr[3]           |
|    x = 10           |
+---------------------+
|     Free space      |
+---------------------+ <- Heap Start
|  malloc(3) -> "hi\0" |
+---------------------+
|  .data / .rodata    |
|  "hi" (string literal) |
+---------------------+
|  .text (code)       |
+---------------------+
Low Address

*/

int main(){
    // stack
    int x = 4; // 4 is stored in stack
    char *y="hi"; // Pointer stored in Stack, string literal stored in Read-Only Data
    // y is stored in stack, it points to string literal ("hi") which is stored in .ro section
    // y[0] = 'H'; // error
    // strcat(y, " there"); // error

     /*
    * It is not modifiable.
    * Often shared among functions if same literal is reused.
    */
/*
   * How to confirm it is read only
!! From `readelf` and `info proc mappings` written only required data
```
$ readelf -S /tmp/pgm.out

[Nr] Name              Type             Address           Offset   Size              EntSize          Flags  Link  Info  Align  
[ 0]                   NULL             0000000000000000  00000000 0000000000000000  0000000000000000           0     0     0
[16] .text             PROGBITS         00000000000010a0  000010a0 0000000000000197  0000000000000000  AX       0     0     16
[17] .fini             PROGBITS         0000000000001238  00001238 000000000000000d  0000000000000000  AX       0     0     4
[18] .rodata           PROGBITS         0000000000002000  00002000 0000000000000009  0000000000000000   A       0     0     4
```
--------------------------

```
$ gdb /tmp/pgm.out /tmp/core.pgm.out.5
(gdb) info proc mappings
    Start Addr           End Addr       Size     Offset objfile
0x5bb09cca2000     0x5bb09cca3000     0x1000        0x0 /tmp/pgm.out
0x5bb09cca3000     0x5bb09cca4000     0x1000     0x1000 /tmp/pgm.out
0x5bb09cca4000     0x5bb09cca5000     0x1000     0x2000 /tmp/pgm.out
0x5bb09cca5000     0x5bb09cca6000     0x1000     0x2000 /tmp/pgm.out
```
-------------------------------------------------------------------------

Mapping starting base address is 0x5bb09cca2000 which is offset (0x0000) 

crash address happened at 0x00005bb09cca322e // this is the address of instruction, where crash is happened, likely text segment
0x00005bb09cca322e - 
0x00005bb09cca2000
----------------------
              122e

Now check which elf section contains Vaddress 0x122e

In elf section 0x122e is likely to fall between `.text` and `.fini`, as per Address and offset

Offset means where the section starts 

- section `.fini` is starts at offset `1238` which is bigger than our crash, so it is definitely not `.fini` section
- .text section is start from offset `000010a0` it's seize is `0000000000000197` to calculate ending offset of text segment `0x10a0 + 0x197 = 0x1237`, so text segment range is `00000000000010a0 - 0000000000001237`
so our offset is 122e which is fall in .text segment

So far we calculated for instruction location 0x00005bb09cca322e

// Now find why SIGSEGV happened 
/**
 * Since program is re-ran address were changed
 *  0x00005bb09cca322e is changed to 0x000055555555522e
 *  0x00005bb09cca2000 is changed to 0x0000555555554000
//--------------------------------------------------------readelf-------------------------------
Section Headers:
[Nr] Name              Type             Address           Offset        Size              EntSize          Flags  Link  Info  Align
[18] .rodata           PROGBITS         0000000000002000  00002000      0000000000000009  0000000000000000   A       0     0     4
[19] .eh_frame_hdr     PROGBITS         000000000000200c  0000200c      000000000000003c  0000000000000000   A       0     0     4

//--------------------------------------------------------readelf------------------------------- 
 

//--------------------------------------------------------GDB-------------------------------
jidesh@jidesh-MS-7E26:~$ gdb /tmp/pgm.out /tmp/core.pgm.out.4932.1758181699 
Program terminated with signal SIGSEGV, Segmentation fault.
#0  0x000055555555522e in main () at stackvsheap.c:75
75	    y[0] = 'H'; // error
(gdb) info proc mappings 
Mapped address spaces:

          Start Addr           End Addr       Size     Offset objfile
      0x555555554000     0x555555555000     0x1000        0x0 /tmp/pgm.out
      0x555555555000     0x555555556000     0x1000     0x1000 /tmp/pgm.out
      0x555555556000     0x555555557000     0x1000     0x2000 /tmp/pgm.out
      0x555555557000     0x555555558000     0x1000     0x2000 /tmp/pgm.out
      0x555555558000     0x555555559000     0x1000     0x3000 /tmp/pgm.out
      .....
(gdb) print y
$1 = 0x555555556004 "hi"
(gdb) print &y
$2 = (char **) 0x7fffffffde98       // address of y, it is in stack
(gdb) print &(y[0])
$3 = 0x555555556004 "hi"
(gdb) print &(y[1])
$4 = 0x555555556005 "i"
(gdb) print &(y[2])
$5 = 0x555555556006 ""


//--------------------------------------------------------GDB-------------------------------

Here SIGSEGV is happend for address 0x555555556004
calculate offset
0x555555556004  -
0x555555554000
---------------
          2004

Now check elf's address where this will fall

.rodata section's start address 0000000000002000 + 0000000000000009 (size) = 0000000000002009 => 2009
2004 is fall under .rodata
.rodata's Flag is A -> means it has only premission to allocate it don't have permission to write(W)

//---------------------------------------------------------OBJDUMP--------------------------------
// we can verify the "hi" string literal in objectdump
jidesh@jidesh-MS-7E26:~$ objdump -s -j .rodata /tmp/pgm.out

/tmp/pgm.out:     file format elf64-x86-64

Contents of section .rodata:
 2000 01000200 686900                      ....hi.  
//---------------------------------------------------------OBJDUMP--------------------------------

// How string literal stored
/*
y is stored in stack, y is a pointer, it points to address of char
y points to 0x555555556004
0x555555556004 -> h
0x555555556005 -> i
0x555555556004 -> "\0"
*/
   
   
    char a = 'a'; // how this will be stored, directly in stack
    /*
Breakpoint 1, main () at stackvsheap.c:205
205	    char a = 'a'; // how this will be stored, directly in stack
(gdb) info line
Line 205 of "stackvsheap.c" starts at address 0x555555555254 <main+45> and ends at 0x555555555258 <main+49>.
(gdb) print a
$2 = 97 'a'
(gdb) print &a
$3 = 0x7fffffffde53 "a\004"

(gdb) info proc mappings 
process 6087
Mapped address spaces:
          Start Addr           End Addr       Size     Offset  Perms  objfile
      0x555555554000     0x555555555000     0x1000        0x0  r--p   /tmp/pgm.out
      0x555555555000     0x555555556000     0x1000     0x1000  r-xp   /tmp/pgm.out
      0x555555556000     0x555555557000     0x1000     0x2000  r--p   /tmp/pgm.out
      0x555555557000     0x555555558000     0x1000     0x2000  r--p   /tmp/pgm.out
      0x555555558000     0x555555559000     0x1000     0x3000  rw-p   /tmp/pgm.out
      0x7ffff7c00000     0x7ffff7c28000    0x28000        0x0  r--p   /usr/lib/x86_64-linux-gnu/libc.so.6
      0x7ffff7c28000     0x7ffff7db0000   0x188000    0x28000  r-xp   /usr/lib/x86_64-linux-gnu/libc.so.6
      0x7ffff7db0000     0x7ffff7dff000    0x4f000   0x1b0000  r--p   /usr/lib/x86_64-linux-gnu/libc.so.6
      0x7ffff7dff000     0x7ffff7e03000     0x4000   0x1fe000  r--p   /usr/lib/x86_64-linux-gnu/libc.so.6
      0x7ffff7e03000     0x7ffff7e05000     0x2000   0x202000  rw-p   /usr/lib/x86_64-linux-gnu/libc.so.6
      0x7ffff7e05000     0x7ffff7e12000     0xd000        0x0  rw-p   
      0x7ffff7f92000     0x7ffff7f95000     0x3000        0x0  rw-p   
      0x7ffff7fbd000     0x7ffff7fbf000     0x2000        0x0  rw-p   
      0x7ffff7fbf000     0x7ffff7fc1000     0x2000        0x0  r--p   [vvar]
      0x7ffff7fc1000     0x7ffff7fc3000     0x2000        0x0  r--p   [vvar_vclock]
      0x7ffff7fc3000     0x7ffff7fc5000     0x2000        0x0  r-xp   [vdso]
      0x7ffff7fc5000     0x7ffff7fc6000     0x1000        0x0  r--p   /usr/lib/x86_64-linux-gnu/ld-linux-x86-64.so.2
      0x7ffff7fc6000     0x7ffff7ff1000    0x2b000     0x1000  r-xp   /usr/lib/x86_64-linux-gnu/ld-linux-x86-64.so.2
      0x7ffff7ff1000     0x7ffff7ffb000     0xa000    0x2c000  r--p   /usr/lib/x86_64-linux-gnu/ld-linux-x86-64.so.2
      0x7ffff7ffb000     0x7ffff7ffd000     0x2000    0x36000  r--p   /usr/lib/x86_64-linux-gnu/ld-linux-x86-64.so.2
      0x7ffff7ffd000     0x7ffff7fff000     0x2000    0x38000  rw-p   /usr/lib/x86_64-linux-gnu/ld-linux-x86-64.so.2
      0x7ffffffde000     0x7ffffffff000    0x21000        0x0  rw-p   [stack]
  0xffffffffff600000 0xffffffffff601000     0x1000        0x0  --xp   [vsyscall]

  0x7fffffffde53 is fall in `0x7ffffffde000     0x7ffffffff000    0x21000        0x0  rw-p   [stack]`
    */
// The stack is not part of the ELF file — it is created and managed at runtime by the OS.
// so we can't inspect in objdump and readelf

//(gdb) print &a1
//$15 = (char (*)[3]) 0x7fffffffde72
    char a1[3] = {'A', 'B', '\0'};
    // a1[0] -> 0x7fffffffde72 -> A
    // a1[1] -> 0x7fffffffde73 -> B
    // a1[2] -> 0x7fffffffde74 -> \0
    // a1[3] -> 0x7fffffffde75 // next -> C
    char a2[3] = "CD"; // initially it is string literal then content will be copied to stack
    // a2[0] -> 0x7fffffffde75 -> C
    // a2[1] -> 0x7fffffffde76 -> D
    a2[0] = 'e';

    // heap
    // stack top
    /* (gdb) print $rsp
       $16 = (void *) 0x7fffffffde50
    */

    /*
    (gdb) print &x1
    $20 = (int **) 0x7fffffffde60
    */
    int *x1 = malloc(sizeof(int));
    *x1 = 4;

    /*
    (gdb) print &y1
    $19 = (char **) 0x7fffffffde68
    */
    char *y1 = malloc(3);  // For "me" + '\0'
    strcpy(y1, "me");
    func();
    printf("At GDB break here");
}