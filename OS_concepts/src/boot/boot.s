;nasm boot.s -f bin -o boot.bin
;cp boot.bin boot.img
;truncate -s 1440k boot.img 

[org 0x7C00]
[BITS 16]

SETUPLEN equ 4				; no.of setup sectors
BOOTSEG  equ 0x07c0			; BIOS address of boot-sector
INITSEG  equ 0x9000			; new boot-sector address
SYSSEG	equ 0x1000			; kernel location
SYSSIZE equ 0x3000			; 196kB
ENDSEG	equ SYSSEG + SYSSIZE ; 1000+3000 => 4000

section .data
	secorts dw 0

section .text
	global _start
	jmp _start

_start:
	mov ax, BOOTSEG
	mov ds, ax
	mov ax, INITSEG
	mov es, ax
	mov cx, 256
	mov si, 0
	mov di, 0
	rep movsw		; copy ds:si to es:di then increment si, di
	jmp INITSEG:go	; once copied jump to new code segment with offset
; from below code Code segment is changed to 9000 and instruction pointer offset address remains same

go: mov ax, cs		
	mov ds, ax		; point ds and es to new code segment
	mov es, ax
	mov ss, ax 		; set stack segment at INITSEG
	mov sp, 0xFF00	; place stack pointer to FF00
	; now address of stack is 0x9FF00

load_setup:
	mov dx, 0x0000 ; dh head 0, dl drive 0
	mov	cx, 0x0002 ; ch cylinder 0 , cl sector 2 
	mov	bx, 0x0200 ; ES points to 0x9000, BX points to 0x0200 ES:BX => 0x90200
	mov	ax, 0x0200+SETUPLEN	 ; 0x0200+4 => 0x0204 , ah = 02, al = 04; ah 02 is read sector from drive
	int	0x13			
	jnc	ok_load_setup		

	mov	dx,0x0000	
	mov	ax,0x0000	;Reset disk		
	int	0x13
	jmp	load_setup

ok_load_setup:
	; Get drive param
	mov	dl,0x00
	mov	ax,0x0800		; AH=8 is get drive parameters
	int	0x13

	mov	ch,0x00
	mov	[sectors],cx  	; CH is cleared and stored no.of. sectors to memory
	mov	ax,INITSEG 	; ax has new boot location
	mov	es,ax
	; seem's like above code is only interested in secorts

	; screen msg 
	; Get page no, row no, column no
    mov	ah,0x03
	int	0x10

	; Printing OS info
	mov	ax, 0x1301      
    mov bp, msg         
	mov cx, msglen      
	mov bl, 0x02 		
	int 0x10

	; Load system at 0x10000

	mov ax, SYSSEG
	mov es, ax

	jmp $

read_it:
	mov ax, es
	test ax, 0x0fff ; set ZF to 1 if ax and (&) #0x0fff return 0
	; if ZF = 0 jump to die
die:
	jne die

	mov bx, 0
rp_read:
	mov ax, es
	cmp ax, ENDSEG
	jb ok1_read
	ret
ok1_read:
	

msg db "ji OS Loading........",10,0
msglen equ $-msg
times 510-($-$$) db 0
dw 0xAA55