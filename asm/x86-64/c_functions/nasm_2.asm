;nasm -f elf64 -o a.o nasm_19.asm && gcc -z noexecstack -m64 -no-pie a.o -o a.out

; functions
; when a function is called the next line address of calling the function will pushed to stack
; for arguments we must use stack it is the good way of doing
; ref https://www.youtube.com/watch?v=21u0Uerx0_Y&list=PL2EF13wm-hWCoj6tUBGUmrkJmH1972dBB&index=24

section .text
    global main

addTwo:
    push rbp
    mov rbp, rsp
    mov rax, [rbp+8]
    mov rbx, [rbp+12]
    add rax, rbx
    pop rbp
    ret
main:
    push 4
    push 2
    call addTwo     ; rax will have return value

; exit
    mov rbx, rax
    mov rax, 1
    int 80h


