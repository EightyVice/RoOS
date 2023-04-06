;
;  Stage 1 Bootloader
;  ================
;  Purpose: - Checking RAM and disk storage
;           - Loading Second Stage Bootloader
;
_:
    bits 16
    org 0x7c00              ; BIOS loads the boot sector into 0x7C00

includes:
    jmp entry
    %include "print.inc"    
   
entry:
    xor ax, ax
    mov ds, ax
    mov es, ax

    mov ax, 0x9000  ;
    mov ss, ax      ; Stack setupping
    mov sp,0xF000   ;

.reset_disk:
    mov si, str_disk_reset
    call Print

    mov ah, 0           ; RESET DISK SYSTEM
    mov dl, 0           ; Drive 0
    int 0x13            
    jc .reset_disk      ; On error, retry

.load_second_stage:
    ; Load second stage of bootloader at address 0x1000:0x0
    mov ax, 0x1000  ;
    mov es, ax      ; Read the sector into address 0x1000:0
    xor bx, bx      ;
   

    mov ah, 0x02    ; READ_DISK_SETOR
    mov al, 1       ; Number of sectors to read
    mov ch, 0       ; Track
    mov cl, 2       ; Sector number (sectors strats from 1)
    mov dh, 0       ; Head number 
    mov dl, 0       ; Drive number (0=A:, 1=2nd floppy, 80h=drive 0, 81h=drive 1)
    int 0x13

    mov si, str_disk_load
    call Print
    jc .load_second_stage   ; On error, retry.

    ; Jump to the Stage 2 sector and start execution
    mov dx, [es:0x0]    ; For debugging purposes print the first 2 bytes
    call print_hex      ; at the second sector for checking if it's loaded correctly.

    mov cx, [es:0x0]                ; First 2 bytes
    cmp cx, 0xB7E9                  ; If the bytes = 0x6DEB 
    jne .correct_sector_data         ; Run the second stage bootloader
    mov si, str_stage2_corrupted    ; Oh no,
    call Print                      ; something wrong! :(
    cli
    hlt                             ; Halt!

.correct_sector_data:
    jmp 0x1000:0x0
    ; Halting will be done at the second stage!


dots db "...", 0
str_stage2_corrupted db "Bootloader corrupted. Halting.", 0xD, 0xA, 0
str_disk_reset db "Resetting Floppy Disk...", 0xD, 0xA, 0
str_disk_load db "Done. Loading stage 2...", 0xD, 0xA, 0


    times 510 - ($-$$) db 0    ; Fill the sector by zeroes
    dw 0xAA55                  ; Boot Signature
