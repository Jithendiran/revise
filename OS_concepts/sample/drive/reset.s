;nasm reset.s -f bin -o boot.img 
;qemu-system-i386 ./boot.img

[org 0x7C00]
[BITS 16]

section .text
	global _start
	jmp _start

_start:

    mov	ah,0x00	   ;Reset disk		
    mov	dl,0x00    ; which drive to reset	
	int	0x13
    ;return CF => 1 if error, 0 if success
    ; AH => return code,
    jnc ok ; jmp to of label if C Flag is 0
    jmp error

ok:
    mov ah, 0x0E
    mov al, 'O'
    mov bh, 0x00
    mov bl, 0x02
    int 10h
    jmp done

error:
    mov ah, 0x0E
    mov al, 'E'
    mov bh, 0x00
    mov bl, 0x02
    int 10h

done:
    jmp $

times 510-($-$$) db 0
dw 0xAA55

; status code
; 00  no error
; 01  bad command passed to driver
; 02  address mark not found or bad sector
; 03  diskette write protect error
; 04  sector not found
; 05  fixed disk reset failed
; 06  diskette changed or removed
; 07  bad fixed disk parameter table
; 08  DMA overrun
; 09  DMA access across 64k boundary
; 0A  bad fixed disk sector flag
; 0B  bad fixed disk cylinder
; 0C  unsupported track/invalid media
; 0D  invalid number of sectors on fixed disk format
; 0E  fixed disk controlled data address mark detected
; 0F  fixed disk DMA arbitration level out of range
; 10  ECC/CRC error on disk read
; 11  recoverable fixed disk data error, data fixed by ECC
; 20  controller error (NEC for floppies)
; 40  seek failure
; 80  time out, drive not ready
; AA  fixed disk drive not ready
; BB  fixed disk undefined error
; CC  fixed disk write fault on selected drive
; E0  fixed disk status error/Error reg = 0
; FF  sense operation failed
