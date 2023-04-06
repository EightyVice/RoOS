bits 16



gdt_data:
    dd 0                ; Null-
    dd 0                ; -descriptr

;   Code Descriptor
    dw 0x0FFFF          ; Segment Low Limit
    dw 0                ; Segment Base Low
    db 0                ; Segment Base Middle
    db 10011010b        ; Access
    db 11001111b        ; Granuality
    db 0                ; Base High

;   Data Descriptor
    dw 0x0FFFF          ; Segment Limit Low
    dw 0                ; Segment Base Low
    db 0                ; Segment Base Middle
    db 10010010b        ; Access
    db 11001111b        ; Granuality
    db 0                ; Base High
    
gdt_end:
toc:
    dw gdt_end - gdt_data - 1   ; Size of GDT
    dd gdt_data                 ; Base of GDT

; Procedure: InstallGDT
; Arugments: None
; - Installs the Global Descriptor Table
InstallGDT:
    cli                 ; Clear Interupts
    pusha               ; Save Registers
    lgdt [toc]          ; Load GDT into GDTR
    sti                 ; Enable Interupts
    popa                ; Restore Registers
    ret                 ; Return
