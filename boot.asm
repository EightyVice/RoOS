;
;  Stage 1 Bootloader
;  ================
;  Purpose: - Checking RAM and disk storage
;           - Loading Second Stage Bootloader
;
_:
    bits 16
    org 0x7c00              ; BIOS loads the boot sector into 0x7C00
                            ; OPINION: ORG 0 then enforce CS:IP for portability


STAGE2_ENTRY equ 0x7E00

entry:
    xor ax, ax  ; ax = ax xor ax ; mov ax, 0
    mov ds, ax
    mov es, ax

    mov ax, 0x9000   ;
    mov ss, ax       ; Stack setupping
    mov sp, 0xF000   ;

.reset_disk:
    mov si, str_disk_reset
    call Print

    mov ah, 0           ; RESET DISK SYSTEM
    mov dl, 0           ; Drive 0
    int 0x13            
    jc .reset_disk      ; On error, retry

.load_second_stage:
    ; Load second stage of bootloader at address 0x1000:0x0
    mov ax, 0                   ;
    mov es, ax                  ; Read the sector into address 0:0xSTAGE2_ENTRY
    mov ax, STAGE2_ENTRY
    mov bx, ax                  ;
   

    mov ah, 0x02    ; READ_DISK_SETOR
    mov al, 1       ; Number of sectors to read
    mov ch, 0       ; Track
    mov cl, 2       ; Sector number (sectors strats from 1)
    mov dh, 0       ; Head number 
    mov dl, 0x80       ; Drive number (0=A:, 1=2nd floppy, 80h=drive 0, 81h=drive 1)
    int 0x13

    mov si, str_disk_load
    call Print
    jc .load_second_stage   ; On error, retry.

    mov si, str_stage2_key
    call Print

    ; Jump to the Stage 2 sector and start execution
    mov dx, [es:0x0]    ; For debugging purposes print the first 2 bytes
    call print_hex      ; at the second sector for checking if it's loaded correctly.

    mov si, str_newline
    call Print

    mov cx, [es:0x0]                ; First 2 bytes
    cmp cx, 0
    je .incorrect_sector_data       ; Run the second stage bootloader
    jmp 0:STAGE2_ENTRY              ; Jump there to execute

    ; Halting will be done at the second stage!


.incorrect_sector_data:
    mov si, str_stage2_corrupted    ; Oh no,
    call Print                      ; something wrong! :(
    cli
    hlt                             ; Halt!


str_newline db 0xD, 0xA, 0
str_stage2_corrupted db "Bootloader corrupted. Halting.", 0xD, 0xA, 0
str_stage2_key db "Stage 2 first two bytes: ", 0

str_disk_reset db "Resetting Floppy Disk...", 0xD, 0xA, 0
str_disk_load db "Done. Loading stage 2...", 0xD, 0xA, 0

    %include "bios_print.inc"    

    times 510 - ($-$$) db 0    ; Fill the sector by zeroes
    dw 0xAA55                  ; Boot Signature
