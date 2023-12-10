;nasm printString.s -f bin -o boot.img 
;qemu-system-i386 ./boot.img

; int 10 is for video mode

[org 0x7C00]
[BITS 16]

section .text
	global _start
	jmp _start

_start:
    mov	ah,0x03		    ; Get cursor position and shape
	int	0x10
    ;return AX = 0, CH = Start scan line, CL = End scan line, DH = Row, DL = Column
;-----------------------------------------------------------------
    ; AH = 13 write string
    ; AL = Write mode (0 - 3)
        ; 0 string is chars only, attribute in BL, cursor not moved
        ; 1 string is chard only, attribute in BL, cursor moved
        ; 2 string contains chars and attributes, cursor not moved
        ; 3 string contains chars and attributes, cursor moved
    ; BH = Page Number
    ; BL = Color
    ; CX = Number of characters in string
    ; DH = Row
    ; DL = Column
    ; ES:BP = Offset of string
	mov	ax, 0x1301      ; ah -> 13 al -> 01
    mov bp, msg         
	mov cx, msgLen      
	mov bl, 0x02 		
	int 0x10
;-------------------------------------------------------------------
    ; mov ah, 0x02        ;Set cursor position
    ; mov bh, 0           ;BH = Page Number
    ; mov dh, 0           ;DH = Row
    ; mov dl, 0           ;DL = Column
    ; int 0x10

    ; move cusrsor to next line
    ; mov ah, 0x02
    ; inc dh    
    ; mov dl, 0           
    ; int 0x10
    ; jmp $

msg db "jith",10,0
msgLen equ $-msg
times 510-($-$$) db 0
dw 0xAA55

; colors
;  - Black            0x00
;  - Blue             0x01
;  - Green            0x02
;  - Cyan             0x03
;  - Red              0x04
;  - Magenta          0x05
;  - Brown            0x06
;  - LightGray        0x07
;  - DarkGray         0x08
;  - LightBlue        0x09
;  - LightGreen       0x0A
;  - LightCyan        0x0B
;  - LightRed         0x0C
;  - LightMagenta     0x0D
;  - LightBrown       0x0E
;  - White            0x0F