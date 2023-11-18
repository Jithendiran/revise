; EQU, inlude
; EQU is used for defining constants for future use
; %include "file.asm"
; %include is used to include files in macro

STRIN equ 0
STDOUT equ 1
STDERR equ 2

SYS_READ equ 0
SYS_WRITE equ 1
SYS_EXIT equ 60

section .data
    text db "Hello",10

section .text
    global _start

_start:
    mov rax, SYS_WRITE
    mov rdi, STDOUT
    mov rsi, text
    mov rdx, 6
    syscall

    mov rax, SYS_EXIT
    mov rdi, 0
    syscall