; print
; nasm -f elf64 -o a.o nasm_2.asm && ld a.o -o a.out

section .data
    text db "Hello world",10
    text2 db "end",10

section .text
    global _start

_start:
    call print
    jmp exit

exit:
    mov rax, 60         
    mov rdi, 0          
    syscall            

print:
    mov rax, 1          
    mov rdi, 1         
    mov rsi, text      
    mov rdx, 12         
    syscall   
    ret       

; when ever call is used used rip will point to the label address and control goes There
; in subroutine when ever ret is encounter control goes back to where it is called

; when ever jmp is encounter, like call rip will point to the label address and control goes There
; but control will not come back

; call without ret is segmentfault

; when a function is called the next line address of calling the function will pushed to stack
; in _start `call print` is the calling the print function the address of `jup exit` is pushed to stack
; when the function finished it's execution it will pop the address and return to it
; we can see the return address is stored in rsp register