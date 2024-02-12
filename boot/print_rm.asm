[BITS 16]

; Real mode print string
; Takes in address of string in %bx
print_string_rm:
    pusha

print_string_rm_loop:
    mov al, [bx]
    cmp al, 0
    je print_string_rm_end

    mov ah, 0x0e    ; BIOS teletype function
    int 0x10        ; BIOS interrupt
    inc bx
    jmp print_string_rm_loop

print_string_rm_end:
    popa
    ret


; Real mode print hexadecimal value
; Takes in the value in %bx
print_hex_rm:
    pusha
    mov cl, 16      ; Bits to shift

print_hex_rm_loop:
    sub cl, 4
    
    mov ax, bx
    shr ax, cl
    and ax, 0x000F

    cmp ax, 9
    jg print_hex_rm_char

print_hex_rm_digit:
    add ax, '0'
    jmp print_hex_rm_print

print_hex_rm_char:
    sub ax, 10
    add ax, 'A'

print_hex_rm_print:
    mov ah, 0x0e    ; BIOS teletype function
    int 0x10        ; BIOS interrupt

    cmp cl, 0
    jne print_hex_rm_loop

print_hex_rm_end:
    popa
    ret


; Real mode print newline
print_newline_rm:
    pusha
    mov al, 0x0a    ; Newline char
    mov ah, 0x0e    ; BIOS teletype function
    int 0x10
    mov al, 0x0d    ; Carriage return
    mov ah, 0x0e    ; BIOS teletype function
    int 0x10
    popa
    ret


; Real mode clear terminal
clear_terminal_rm:
    pusha
    mov ah, 0x00    ; Function number for setting video mode
    mov al, 0x03    ; Video mode 0x03 (80x25 text mode)
    int 0x10        ; BIOS interrupt
    popa
    ret