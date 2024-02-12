[org 0x7c00]    ; Standard memory location of boot sector
[bits 16]       ; 16 bit mode

; CODE
mov [BOOT_DRIVE], dl

; Set the stack safely away
mov bp, 0x9000
mov sp, bp

; Announce real mode
mov bx, REAL_MODE_MSG
call print_string_rm

; Load the kernel
mov bx, KERNEL_LOAD_MSG
call print_string_rm
mov bx, KERNEL_OFFSET
mov dh, SECTOR_LOAD_COUNT
mov dl, [BOOT_DRIVE]
call disk_load

; Set output to video mode instead of terminal
mov ah, 0x00    ; Function: Set Video Mode
mov al, 0x13    ; Mode: 320x200 256-color mode
int 0x10        ; Call BIOS

; Go to protected mode
call switch_to_pm ; We never return from here

[bits 32]
BEGIN_PM:
    mov ebx, PROTECTED_MODE_MSG
    call print_string_pm
    call KERNEL_OFFSET
    jmp $

; INCLUDES
%include "boot/print_rm.asm"
%include "boot/disk.asm"
%include "boot/gdt.asm"
%include "boot/pm_switch.asm"
%include "boot/print_pm.asm"


; CONSTANTS
BOOT_DRIVE db 0
REAL_MODE_MSG db "Started in 16-bit real mode.", 0x0A, 0x0D, 0
KERNEL_LOAD_MSG db "Loading kernel into memory.", 0x0A, 0x0D, 0
PROTECTED_MODE_MSG db "Landed in 32-bit protected mode.", 0
SECTOR_LOAD_COUNT equ 16
KERNEL_OFFSET equ 0x1000


times 510 - ($-$$) db 0     ; Pad the rest of the boot sector with zeros
dw 0xaa55                   ; Boot sector signature at the end