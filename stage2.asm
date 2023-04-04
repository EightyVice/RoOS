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

heystr db "Hello from second stage",0

entry:
    mov ax, cs      ; For god sake never remove that! 
    mov ds, ax      ; enforce the segments from the Code Segment (CS)
    mov es, ax      ; since data and code are in the same segment.

    mov si, heystr
    call Print

    cli
    hlt
    times 512 - ($-$$) db 0    ; Fill the sector by zeroes and no boot signature needed. it's our sector


