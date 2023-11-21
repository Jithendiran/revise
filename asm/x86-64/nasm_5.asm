; stack
; stack is a temprory storage
; push will put the content to the top of the stack
; pop will throw out the content from top of the stack
; rip will hold the address of the stack rip is a pointer
; to get the value of rip use [rip]
; [] means pointer

section .data
    text db "xcx cv",10

section .bss
    tex resb 16
    ;tex db "scxsclsc" ;it will raise warning: attempt to initialize memory in BSS section `.bss': ignored [-w+other]

section .text
    global _start

_start:
    ;mov [text], "hi ji" 
    ;mov [tex], "hi ji"      ; not working because it does not know the size 
    call print
    push 1
    push 2
    push 3

    pop rax
    call printRAXDigit
    jmp exit

exit:
    mov rax, 60         
    mov rdi, 0          
    syscall  

print:
    mov rax, 1          
    mov rdi, 1
    mov rsi, text      
    mov rdx, 20         
    syscall 
    ret 

printRAXDigit:
    mov [text], rax 
    mov rax, 1          
    mov rdi, 1
    mov rsi, text      
    mov rdx, 20         
    syscall 
    ret  