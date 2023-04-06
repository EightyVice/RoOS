;
;   Stage 2 Bootloader
;   ==================
;   Purpose: - Setting GDT
;            - Switching to Protected Mode (kernel mode)
;            - Loading Kernel
_:
    bits 16
    org 0   ; IMPORTANT: originate from zero since 
            ; we append the stages into the sectors anyway


includes:
    jmp entry
    %include "print.inc"
    %include "gdt.inc"

heystr db "Hello from second stage", 0xD, 0xA, 0x0
str_loading_os db "Loading Operating System...", 0xD, 0xA, 0x0
entry:
    cli
    mov ax, cs      ; For god sake never remove that! 
    mov ds, ax      ; enforce the segments from the Code Segment (CS)
    mov es, ax   
    mov ax, 0x9000
    mov ss, ax
    mov sp, 0xFFFF
    sti

    mov si, heystr
    call Print

    ; Install Global Descriptor Table
    ;InstallGDT:
    cli                 ; Clear Interupts
    pusha               ; Save Registers
    ;lgdt [toc]          ; Load GDT into GDTR
    ;sti                 ; Enable Interupts
    popa                ; Restore Registers
    ;ret                 ; Return


    ; Convert into Protected Mode
    cli                 ; Clear Interupts
    mov eax, cr0        ; Set bit 0 in CR0 (Enter Protected Mode)
    or  eax, 1          
    mov cr0, eax
    
    jmp 0x8:pmode
;======================================
;       32-bit Protected Mode
;======================================
bits 32
pmode:
    mov ax, 0x10
    mov ds, ax
    mov ss, ax
    mov es, ax
    mov esp, 0x90000



    cli
    hlt
    times 512 - ($-$$) db 0    ; Fill the sector by zeroes and no boot signature needed. it's our sector


