; basic

; compile: nasm -f elf64 -o nasm_1.o nasm_1.asm
; ld nasm_1.o -o nasm_1
; objdump -D nasm_1 ; to check compiled code
; for debugging set : echo "set disassembly-flavor intel" > ~/.gdbinit

section .data
        text db "Hello",10
        ; db -> define bytes
        ; it means that we are going to define some raw bytes of data to insert into our code
        ; each character is a bytes "h" is a byte, "e" is a byte, ... 10 means new line
        ; text is a name assigned to the address in memory where "hello" data is stored
        ; when ever the text is encounter compiler will replace with actual memory address

section .text
        global _start
; global _start is where the program need to start 

; _start: is a label
; label| code |          rip pointer|   desc
_start:                     
        mov rax, 1          ;rip = x (x is a address of the insturction)
        mov rdi, 1          ;rip = x+1
        mov rsi, text       ;rip = x+2   ; value to print 
        mov rdx, 6          ;rip = x+3   ; legth to print, length is 6 because "hello" word count is 5 and "\n" count is 1 total 6
        syscall             ;rip = x + 4 ; based on the rax and it arguments system calls are called 

        mov rax, 60         ;rip = x + 5 ; 60 is sys_exit
        mov rdi, 0          ;rip = x + 6
        syscall             ;rip = x+7 


; mov is moving the value from right to left

; sys_write ID is 1 which is stored in rax register
; fd is file descriptor 0 -> standard input, 1 -> standard output, 2 -> standard error. which is stored in rdi register
; buf is location of memory address. the address of string which is going to write is stored in rsi register
; count is number of content used for operation. the number of bytes to print which is stored in rdx register

; sys_exit takes 1 argument which is error code
; error code can be any number, 0 means no error

; program without sys_exit will segmentfault

; if we not specify _start; code will start execute from predefined address 0000000000401000

; below is warning if we not specify _start
; ld: warning: cannot find entry symbol _start; defaulting to 0000000000401000