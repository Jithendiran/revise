/*
* Author: E.K.Jithendiran
* Date: 19 Sep 2025
*/
// echo 0 | sudo tee /proc/sys/kernel/randomize_va_space // use this to disable ALSR
// gcc -g -O0 stringliteral.c -o /tmp/pgm.out
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
/*
How string literal is stored
char *y = "hi"; is different from char y[] = "hi"; or char *y = {'h', 'i', '\0'};
*y = "hi" is consider as string literal, it is stored in read-only section, remaining are consider as character array, stored in stack

Since string literal is stored in read-only section we can't modify
*/

int main() {
    char *y="hi"; // y is stored in stack, it is a pointer to string literal
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

//-----------------------------------------------------------------------------------------------------------------------------------------------------------------
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
    char a2[3] = "CD"; 
    // a2[0] -> 0x7fffffffde75 -> C
    // a2[1] -> 0x7fffffffde76 -> D
    a2[0] = 'e';

/*
objdump -d   /tmp/pgm.out
0000000000001149 <main>:
    1149:	f3 0f 1e fa          	endbr64
    114d:	55                   	push   %rbp
    114e:	48 89 e5             	mov    %rsp,%rbp
    1151:	48 83 ec 20          	sub    $0x20,%rsp
    1155:	64 48 8b 04 25 28 00 	mov    %fs:0x28,%rax
    115c:	00 00 
    115e:	48 89 45 f8          	mov    %rax,-0x8(%rbp)
    1162:	31 c0                	xor    %eax,%eax
    1164:	48 8d 05 99 0e 00 00 	lea    0xe99(%rip),%rax        # 2004 <_IO_stdin_used+0x4>
    116b:	48 89 45 e8          	mov    %rax,-0x18(%rbp)
    116f:	c6 45 e7 61          	movb   $0x61,-0x19(%rbp)
    1173:	66 c7 45 f2 41 42    	movw   $0x4241,-0xe(%rbp)   # 41 -> A, 42 -> B
    1179:	c6 45 f4 00          	movb   $0x0,-0xc(%rbp)      # 0  -> \0
    117d:	66 c7 45 f5 43 44    	movw   $0x4443,-0xb(%rbp)   # 43 -> C, 44 -> D
    1183:	c6 45 f7 00          	movb   $0x0,-0x9(%rbp)
    1187:	c6 45 f5 65          	movb   $0x65,-0xb(%rbp)
    118b:	b8 00 00 00 00       	mov    $0x0,%eax
    1190:	48 8b 55 f8          	mov    -0x8(%rbp),%rdx
    1194:	64 48 2b 14 25 28 00 	sub    %fs:0x28,%rdx
    119b:	00 00 
    119d:	74 05                	je     11a4 <main+0x5b>
    119f:	e8 ac fe ff ff       	call   1050 <__stack_chk_fail@plt>
    11a4:	c9                   	leave
    11a5:	c3                   	ret

*/
}