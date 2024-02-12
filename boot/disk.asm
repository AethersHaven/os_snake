[bits 16]
disk_load:
    pusha

    push dx

    mov ah, 0x02    ; Read operation
    mov al, dh      ; Number of sectors to read
    mov cl, 0x02    ; Sector
    mov ch, 0x00    ; Cylinder
    mov dh, 0x00    ; Head

    int 0x13        ; BIOS interrupt
    jc disk_error

    pop dx
    cmp al, dh      ; Check if actual # of sectors read is equal to requested
    jne sector_error

    popa
    ret

disk_error:
    mov bx, DISK_ERROR_MSG
    call print_string_rm
    jmp $

sector_error:
    mov bx, SECTOR_ERROR_MSG
    call print_string_rm
    jmp $


DISK_ERROR_MSG db "Disk read error!", 0
SECTOR_ERROR_MSG db "Incorrect number of sectors read!", 0