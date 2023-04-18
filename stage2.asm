;
;   Stage 2 Bootloader
;   ==================
;   Purpose: - Setting GDT
;            - Switching to Protected Mode (kernel mode)
;            - Loading Kernel
_:
    bits 16
    org 0x7E00


entry:
	; 0x10000
    mov ax, 0x1000                   ;
    mov es, ax                  ; Read the sector into address 0:0xSTAGE2_ENTRY
    mov ax, 0
    mov bx, ax                  ;

.load_kernel:
    mov ah, 0x02    ; READ_DISK_SETOR
    mov al, 10      ; Number of sectors to read
    mov ch, 0       ; Track
    mov cl, 3       ; Sector number (sectors strats from 1)
    mov dh, 0       ; Head number 
    mov dl, 0x80    ; Drive number (0=A:, 1=2nd floppy, 80h=drive 0, 81h=drive 1)
    int 0x13
    jc .load_kernel   ; On error, retry.


	cli				; clear interrupts
	xor	ax, ax			; null segments
	mov	ds, ax
	mov	es, ax
	mov	ax, 0x9000		; stack begins at 0x9000-0xffff
	mov	ss, ax
	mov	sp, 0xFFFF
	sti				; enable interrupts

	; Install GDT
	call InstallGDT

	; Enable Gate A20 Address Line for accessing
	; 4 gigabyte of RAM (0-0xFFFFFFFF)
	cli
	push ax
	mov  al, 0xdd	; send enable a20 address line command to controller
	out	 0x64, al
	pop	 ax

	cli				; clear interrupts
	mov	eax, cr0		; set bit 0 in cr0--enter pmode
	or	eax, 1
	mov	cr0, eax


	; Disable the cursor
	mov dx, 0x3D4
	mov al, 0xA	; low cursor shape register
	out dx, al
 
	inc dx
	mov al, 0x20	; bits 6-7 unused, bit 5 disables the cursor, bits 0-4 control the cursor shape
	out dx, al


	jmp	08h:PMODE




bits 32
PMODE:
	; -----------------------
	; Start of Protected Mode
	; -----------------------


	mov		ax, 0x10		; set data segments to data selector (0x10)
	mov		ds, ax
	mov		ss, ax
	mov		es, ax
	mov		esp, 90000h		; stack begins from 90000h



	call ClearScreen

	mov ebx, str_welcome
	call PrintString

	mov ebx, str_loading_os
	call PrintString
	


	; Call Kernel (exists there)
	jmp 0x10000
	

STOP: 
	cli
	hlt

hey_str db "Hello from second stage", 0xD, 0xA, 0x0
str_loading_os db "Loading Operating System...", 0xA, 0x0
str_welcome db "                           << Welcome to RoOS >>", 0xA, 0x0

	;%include "bios_print.inc"
	%include "gdt.inc"
	%include "stdio.inc"

    times 512 - ($-$$) db 0    ; Fill the sector by zeroes and no boot signature needed. it's our sector


