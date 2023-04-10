;
; gdt.inc
; 
; Purpose: Setting up the global descriptor talble.
 
%ifndef __GDT_INC_67343546FDCC56AAB872_INCLUDED__
%define __GDT_INC_67343546FDCC56AAB872_INCLUDED__
 
bits	16
 
;*******************************************
; InstallGDT()
;	- Install our GDT
;*******************************************
 
InstallGDT:
 
	cli				; clear interrupts
	pusha				; save registers
	lgdt 	[toc]			; load GDT into GDTR
	sti				; enable interrupts
	popa				; restore registers
	ret				; All done!
 
;*******************************************
; Global Descriptor Table (GDT)
;*******************************************
 
gdt_data: 
	dq 0 				; null descriptor

; Kernel Space Code Descriptor
	dw 0FFFFh 			; limit low
	dw 0 				; base low
	db 0 				; base middle
	db 10011010b 			; access
	db 11001111b 			; granularity
	db 0 				; base high
 
; Kernel Space Data Descriptor
	dw 0FFFFh 			; limit low (Same as code)
	dw 0 				; base low
	db 0 				; base middle
	db 10010010b 		; access
	db 11001111b 		; granularity
	db 0				; base high
 
; User Space code
	dw 0FFFFh 			; limit low
	dw 0 				; base low
	db 0 				; base middle
	db 11111010b 		; access - Notice that bits 5 and 6 (privilege level) are 11b for Ring 3
	db 11001111b 		; granularity
	db 0 				; base high
 
; User Space data
	dw 0FFFFh 			; limit low (Same as code)
	dw 0 				; base low
	db 0 				; base middle
	db 11110010b 		; access - Notice that bits 5 and 6 (privilege level) are 11b for Ring 3
	db 11001111b 		; granularity
	db 0				; base high
	
end_of_gdt:
toc: 
	dw end_of_gdt - gdt_data - 1 	; limit (Size of GDT)
	dd gdt_data 			; base of GDT
 
 
%endif ;__GDT_INC_67343546FDCC56AAB872_INCLUDED__