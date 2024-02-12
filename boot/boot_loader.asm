[org 0x7c00]    ; Standard memory location of boot sector
[BITS 16]       ; 16 bit mode

; CODE
mov bp, 0x9000  ; Set the stack safely away
mov sp, bp

mov bx, REAL_MODE_MSG
call print_string_rm

mov bx, 0xA3C4
call print_hex_rm
call print_newline_rm


; INCLUDES
%include "boot/print_rm.asm"


; CONSTANTS
REAL_MODE_MSG db "Started in 16-bit real mode.", 0x0A, 0x0D, 0


times 510 - ($-$$) db 0     ; Pad the rest of the boot sector with zeros
dw 0xaa55                   ; Boot sector signature at the end