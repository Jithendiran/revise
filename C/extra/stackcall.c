/*
* Author: E.K.Jithendiran
* Date: 20 Sep 2025
*/
// echo 0 | sudo tee /proc/sys/kernel/randomize_va_space // use this to disable ALSR
// gcc -g -O0 stackcall.c -o /tmp/pgm.out

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void func1(char c, int a){}

void func(int a, char b) {
    func1(b, a);
}

void main(){
    func(1, 65);
}

/*
$ objdump -d -m intel  /tmp/pgm.out
0000000000001129 <func1>:
    1129:       f3 0f 1e fa             endbr64
    112d:       55                      push   %rbp
    112e:       48 89 e5                mov    %rsp,%rbp
    1131:       89 f8                   mov    %edi,%eax
    1133:       89 75 f8                mov    %esi,-0x8(%rbp)
    1136:       88 45 fc                mov    %al,-0x4(%rbp)
    1139:       90                      nop
    113a:       5d                      pop    %rbp
    113b:       c3                      ret

000000000000113c <func>:
    113c:       f3 0f 1e fa             endbr64
    1140:       55                      push   %rbp
    1141:       48 89 e5                mov    %rsp,%rbp
    1144:       48 83 ec 08             sub    $0x8,%rsp
    1148:       89 7d fc                mov    %edi,-0x4(%rbp)
    114b:       89 f0                   mov    %esi,%eax
    114d:       88 45 f8                mov    %al,-0x8(%rbp)
    1150:       0f be 45 f8             movsbl -0x8(%rbp),%eax
    1154:       8b 55 fc                mov    -0x4(%rbp),%edx
    1157:       89 d6                   mov    %edx,%esi
    1159:       89 c7                   mov    %eax,%edi
    115b:       e8 c9 ff ff ff          call   1129 <func1>
    1160:       90                      nop
    1161:       c9                      leave
    1162:       c3                      ret

0000000000001163 <main>:
    1163:       f3 0f 1e fa             endbr64
    1167:       55                      push   %rbp
    1168:       48 89 e5                mov    %rsp,%rbp
    116b:       be 41 00 00 00          mov    $0x41,%esi
    1170:       bf 01 00 00 00          mov    $0x1,%edi
    1175:       e8 c2 ff ff ff          call   113c <func>
    117a:       90                      nop
    117b:       5d                      pop    %rbp
    117c:       c3                      ret

// ---------------------------------------------------------------------------------
Explaination
Each function call has it's own stack frame
Stack frame holds
    1. return address
    2. saves old base pointer (rbp)
rsp — Stack Pointer: Points to the top of the stack, grows downward, get's update when `push` and `pop`
rbp — Base Pointer: Anchors the current stack frame
        Why rbp required?
            rsp change when push or pop is called so it would be difficult to use this as a fixed ponit for taking local variables
            When new function is called, rbp is pointed to top of the stack (function address) then rbp is a fixed point, will use
            this to access local variable by using offset

size of rsp and rbp is 8bytes (64bits) 

main() -> func() -> func1()

prgram start
main
Before call:
1000    +-----------------------------+
        | return addr to CRT/OS      |
992     +-----------------------------+ <- rsp, rbp

Main call: (push return address and jump to function)
1000    +-----------------------------+ 
        | return addr to CRT/OS       |
992     +-----------------------------+
        | return address start        |
984     +-----------------------------+ <- rsp

After push rbp: [inside main]
1000    +-----------------------------+ 
        | return addr to CRT/OS       |
992     +-----------------------------+ <-rbp
        | return address start        |
984     +-----------------------------+ 
        | push rbp (992)              |         
976     +-----------------------------+ <-rsp

After mov rbp, rsp:
    rbp = 976
1000    +-----------------------------+ 
        | return addr to CRT/OS       |
992     +-----------------------------+ 
        | return address start        |
984     +-----------------------------+ 
        | push rbp (992) _start rbp   |         
976     +-----------------------------+ <-rsp, rbp

Now have to make a call for func, it has two param
esi will point to 65
edi will point to 1

Now call func: [call will push address 117a and do jump to func]
1000    +-----------------------------+ 
        | return addr to CRT/OS       |
992     +-----------------------------+ 
        | return address start        |
984     +-----------------------------+ 
        | push rbp (992)_start rbp    |         
976     +-----------------------------+ <-rbp (main)
        | return address main(117a)   |
968     +-----------------------------+ <-rsp

Now func is called, instructions from inside func

1. push   %rbp
1000    +-----------------------------+ 
        | return addr to CRT/OS       |
992     +-----------------------------+ 
        | return address start        |
984     +-----------------------------+ 
        | push rbp (992)_start rbp    |         
976     +-----------------------------+ <-rbp (main)
        | return address main(117a)   |
968     +-----------------------------+---------------------func
        | push   %rbp (976)(main)     |
962     +-----------------------------+ <-rsp

2. mov    %rsp,%rbp

1000    +-----------------------------+ 
        | return addr to CRT/OS       |
992     +-----------------------------+ 
        | return address start        |
984     +-----------------------------+ 
        | push rbp (992)_start rbp    |         
976     +-----------------------------+ -- rbp (main)
        | return address main(117a)   |
968     +-----------------------------+----------------------func
        | push   %rbp (976)(mian)     |
960     +-----------------------------+ <-rsp, rbp

current rbp = 960

3. sub    $0x8,%rsp 
It is making room for local variable and parameters
why sub 8? stack grow downward so subtract, 8 is due to  int (4 bytes), padding 3 bytes, char (1 bytes) so 4 + 3 + 1 = 8   

1000    +-----------------------------+ 
        | return addr to CRT/OS       |
992     +-----------------------------+ 
        | return address start        |
984     +-----------------------------+ 
        | push rbp (992)_start rbp    |         
976     +-----------------------------+ --rbp (main)
        | return address main(117a)   |
968     +-----------------------------+-----------------------func
        | push   %rbp (976)(main)     |
960     +-----------------------------+ <-rbp (func)
        | sub    $0x8,%rsp            |
952     +-----------------------------+ <-rsp

current rbp = 960

4. mov %edi, -0x4(%rbp)
rbp = 960 
960 - 4 = 956

960     +-----------------------------+ <-rbp (func)
        |         int                 |
959     +-----------------------------+ 
        |         int                 |
958     +-----------------------------+ 
        |         int                 |
957     +-----------------------------+ 
        |         int                 |
956     +-----------------------------+ <- edi (956-959 int 1)

5. mov    %esi,%eax
store char param to eax

6. mov    %al,-0x8(%rbp)
rbp = 960 
960 - 8 = 952
Store lower byte of eax (al) (which has esi = 0x41) into [rbp - 8]

956     +-----------------------------+ <- int
        |                             |
955     +-----------------------------+ 
        |                             |
954     +-----------------------------+ 
        |                             |
953     +-----------------------------+ 
        |          char               |
952     +-----------------------------+ <-rsp

7. movsbl -0x8(%rbp),%eax
Move Signed Byte to Long (i.e., 8-bit -> 32-bit, with sign extension)
-0x8(%rbp) = the location where the char was stored
%eax = lower 32 bits of %rax (a 32-bit register)
eax = 0x00000041
conversion is happened Because later you pass it as a signed int in a function call, and want to preserve its original sign.

8. mov    -0x4(%rbp),%edx
Simply loads the 4-byte int value into edx

9. Now prepare function param for func1
mov    %edx,%esi
mov    %eax,%edi

10. call   1129 <func1>
Push 1160 to stack and jump to func1
1000    +-----------------------------+ 
        | return addr to CRT/OS       |
992     +-----------------------------+ 
        | return address start        |
984     +-----------------------------+ 
        | push rbp (992)_start rbp    |         
976     +-----------------------------+ --rbp (main)
        | return address main(117a)   |
968     +-----------------------------+----------------------func
        | push   %rbp (976)           |
960     +-----------------------------+ <-rbp (func)
        | sub    $0x8,%rsp            |
952     +-----------------------------+
        | Push 1160                   |
944     +-----------------------------+ <-rsp

Inside func1
11. push   %rbp
1000    +-----------------------------+ 
        | return addr to CRT/OS       |
992     +-----------------------------+ 
        | return address start        |
984     +-----------------------------+ 
        | push rbp (992)_start rbp    |         
976     +-----------------------------+ --rbp (main)
        | return address main(117a)   |
968     +-----------------------------+----------------------func
        | push   %rbp (976)           |
960     +-----------------------------+ <-rbp (func)
        | sub    $0x8,%rsp            |
952     +-----------------------------+
        | Push 1160                   |
944     +-----------------------------+
        | Push %rbp(960)(func)        |
936     +-----------------------------+ <-rsp

12. mov    %rsp,%rbp
1000    +-----------------------------+ 
        | return addr to CRT/OS       |
992     +-----------------------------+ 
        | return address start        |
984     +-----------------------------+ 
        | push rbp (992)_start rbp    |         
976     +-----------------------------+ --rbp (main)
        | return address main(117a)   |
968     +-----------------------------+-----------------------func
        | push   %rbp (976)           |
960     +-----------------------------+ --rbp (func)
        | sub    $0x8,%rsp            |
952     +-----------------------------+
        | Push 1160                   |
944     +-----------------------------+
        | Push %rbp(960)(func)        |
936     +-----------------------------+ <-rsp, rbp

13. mov    %edi,%eax
Copies the first argument (%edi, which is 32-bit, coming from %rdi) into the %eax register.
%eax is the lower 32 bits of the return value register %rax.
This likely means that the return value of this function will be the first parameter.

14. mov %esi, -0x8(%rbp)
(int  a)
Stores the second argument (%esi, from %rsi) at offset -8 from the base pointer.
This puts the second argument into a local variable on the stack.

15.mov %al, -0x4(%rbp)
Moves the lowest byte of %eax (%al) into stack location -0x4(%rbp).
%al is the lowest 8 bits of the return value, which was set from the first argument.

16.nop
NOP (No Operation) — does nothing. Likely inserted for alignment, padding, or debugging purposes.

17. pop %rbp
Restores the previous base pointer from the stack.

1000    +-----------------------------+ 
        | return addr to CRT/OS       |
992     +-----------------------------+ 
        | return address start        |
984     +-----------------------------+ 
        | push rbp (992)_start rbp    |         
976     +-----------------------------+ --rbp (main)
        | return address main(117a)   |
968     +-----------------------------+----------------------func
        | push   %rbp (976)           |
960     +-----------------------------+ --rbp (func)
        | sub    $0x8,%rsp            |
952     +-----------------------------+
        | Push 1160                   |
944     +-----------------------------+ <-rsp


rbp = 960

18. ret
Pop the content and get return address, Returns to the address 1160
The return value will be in %eax, as set earlier.
1000    +-----------------------------+ 
        | return addr to CRT/OS       |
992     +-----------------------------+ 
        | return address start        |
984     +-----------------------------+ 
        | push rbp (992)_start rbp    |         
976     +-----------------------------+ --rbp (main)
        | return address main(117a)   |
968     +-----------------------------+-------------------func
        | push   %rbp (976)           |
960     +-----------------------------+ --rbp (func)
        | sub    $0x8,%rsp            |
952     +-----------------------------+ <- rsp

18. nop (func)

19. leave
leave is the shorthand for following isnatruction
mov %rbp, %rsp (Restores the stack pointer (%rsp) to the base pointer (%rbp), effectively discarding local variables.)
pop %rbp (Restore rbp)

1000    +-----------------------------+ 
        | return addr to CRT/OS       |
992     +-----------------------------+ 
        | return address start        |
984     +-----------------------------+ 
        | push rbp (992)_start rbp    |         
976     +-----------------------------+ --rbp (main)
        | return address main(117a)   |
968     +-----------------------------+ <-rbp(main), rsp

rbp = 976

20. ret
pop the return address and return to it
return to address 117a and pop the stack 

1000    +-----------------------------+ 
        | return addr to CRT/OS       |
992     +-----------------------------+ 
        | return address start        |
984     +-----------------------------+ 
        | push rbp (992)_start rbp    |         
976     +-----------------------------+ --rbp (main), rsp

21 nop (func) 

22. pop %rbp
1000    +-----------------------------+ 
        | return addr to CRT/OS       |
992     +-----------------------------+ 
        | return address start        |
984     +-----------------------------+ <--rsp

rbp = 992

23. ret
1000    +-----------------------------+ 
        | return addr to CRT/OS       |
992     +-----------------------------+ 
*/

/*
Note: 
Stack Address Sequence: Your address numbers (like 1000, 992, 984, etc.) are hypothetical, but they should decrease consistently by 8 (64-bit word size) per push.
CRT means C Runtime
*/