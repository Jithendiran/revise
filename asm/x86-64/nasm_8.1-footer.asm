exit:
    mov rax, SYS_EXIT
    mov rdi, 0
    syscall

print:
    mov rax, SYS_WRITE
    mov rdi, STDOUT
    mov rsi, text
    mov rdx, 6
    syscall
    ret