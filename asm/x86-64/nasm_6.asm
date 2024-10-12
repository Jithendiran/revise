section .data
    text db "Hello, World",10,0 ;
    text2 db "Hii",10,0


section .text
    global _start
 
_start:
    mov rax, text
    call _print

    mov rax, text2
    call _print
 
    jmp exit

exit:
    mov rax, 60         
    mov rdi, 0          
    syscall  
 
 
;input: rax as pointer to string
;output: print string at rax

_print:
    push rax                ; move the all contents of rax to stack
    mov rbx, 0              ; rbx init to 0


_printLoop:
    inc rax                 ; move rax by 1 byte 
    inc rbx                 ; increment rbx
    mov cl, [rax]           ; move the of rax to lower 8bit of rcx, cl will have current character
    cmp cl, 0               ; comapre cl is 0
    jne _printLoop          ; if cl is non zero jump to _printLoop
 
    mov rax, 1              ; when control enter here which mean it has length of the string
    mov rdi, 1              
    pop rsi                 ; stack has the entire content of the text, now pop the stack to rsi
    mov rdx, rbx
    syscall
 
    ret