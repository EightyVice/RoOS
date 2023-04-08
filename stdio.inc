bits 32


VIDEO_MEMORY equ 0xB8000
COLS equ 80
LINES equ 25
ATTRIB equ 63 ; White text on blue blackground

_curX db 0
_curY db 0

; PutChar
;   - Prints  a character to screen
;  BL: Character to print
PutChar:
    pusha                       ; Save registers
    mov edi, VIDEO_MEMORY       ; set EDI to Video memory

    xor eax, eax

    mov ecx, COLS * 2       ; Mode 7 has 2 byres per char, so COLS*2
    mov al, byte [_curY]    ; Get Y Position
    mul ecx                 ; EAX = AL * ECX (y * COLS)
    push eax                ; Save EAX

    mov al, byte [_curX]    ; multiply _curX * 2
    mov cl, 2               ; ...
    mul cl                  ; ...
    pop ecx                 ; ECX = y * COLS
    add eax, ecx            ; EAX = EAX + ECX = y * COLS + x

    xor ecx, ecx
    add edi, eax            ; Add it to base address

    cmp bl, 0x0A
    je .Row

    mov dl, bl              ; Get character    
    mov dh, ATTRIB          ; Set character Attribute
    mov word [edi], dx      ; Address at Video Memory = DX(DH.DL)

    inc byte [_curX]
    jmp .Done

.Row:
    mov byte [_curX], 0     ; Carriage Return
    inc byte [_curY]        ; Line Feed

.Done:
    popa
    ret


;   PrintString
;       - Prints a null terminated string
;   EBX: Address of the null terminated string to print
PrintString:
    pusha
    push ebx
    pop edi         ; EBX = EDI now


.Loop:
    mov bl, byte [edi] ;get next char
    cmp bl, 0
    je .Done

    call PutChar

    inc edi
    jmp .Loop


.Done:
    mov bh, byte [_curX]
    mov bl, byte [_curY]

    call MoveCursor
    popa
    ret


;   ClearScreen
;       - Clears screen
;   No args
ClearScreen:
    pusha                   ; Save all reegisters
    cld                     ; Clear Direction Flag (DF)
    mov edi, VIDEO_MEMORY
    mov cx, 2000
    mov ah, ATTRIB
    mov al,  ' '
    rep stosw

    mov byte [_curX], 0
    mov byte [_curY], 0
    popa
    ret


;   MoveCursor
;       - Update the hardware cursor
;   BH: Y Position
;   BL: X Position

MoveCursor:
    pusha

    xor eax, eax
    mov ecx, COLS
    mov al, bh
    mul ecx
    add al, bl
    mov ebx, eax

    mov al, 0x0F
    mov dx, 0x03D4
    out dx, al

    mov al, bl
    mov dx, 0x03D5
    out dx, al

    xor eax, eax
    mov al, 0x0E
    mov dx, 0x03D4
    out dx, al

    mov al, bh
    mov dx, 0x03D5
    out dx, al

    popa
    ret


;   SetCursor
;       - Set's Cursor's X,Y position
;   AL: X Position
;   AH: Y Position
SetCursor:
    pusha
    mov [_curX], al
    mov [_curY], ah
    popa
    ret



