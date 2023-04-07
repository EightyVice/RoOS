;
;   Stage 2 Bootloader
;   ==================
;   Purpose: - Setting GDT
;            - Switching to Protected Mode (kernel mode)
;            - Loading Kernel
_:
    bits 16
    org 0x1000


entry:
	cli				; clear interrupts
	xor	ax, ax			; null segments
	mov	ds, ax
	mov	es, ax
	mov	ax, 0x9000		; stack begins at 0x9000-0xffff
	mov	ss, ax
	mov	sp, 0xFFFF
	sti				; enable interrupts

	mov si, hey_str
	call Print

	; Install GDT
	call InstallGDT

	cli				; clear interrupts
	mov	eax, cr0		; set bit 0 in cr0--enter pmode
	or	eax, 1
	mov	cr0, eax

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


STOP: 
	cli
	hlt

hey_str db "Hello from second stage", 0xD, 0xA, 0x0
str_loading_os db "Loading Operating System...", 0xD, 0xA, 0x0

	%include "print.inc"
	%include "gdt.inc"

    times 512 - ($-$$) db 0    ; Fill the sector by zeroes and no boot signature needed. it's our sector


