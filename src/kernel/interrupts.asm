section _TEXT class=CODE

KERNEL_SEGMENT equ 0x7e00
extern _main_

global _store_idt_addr
_store_idt_addr:
	push bp
	mov bp,sp

	push bx
	mov bx, [bp+4]

	sidt [ds:bx] 

	mov ax, bx
	pop bx

	mov sp, bp
	pop bp
ret


global _set_ivt
_set_ivt:
	push bp
	mov bp, sp
	cli
	mov ax, 0
	mov es, ax
	mov cx, 1
	mov bx, word [bp+4] 
	lea si, [bp+6]
	lodsw 

	mov word [es:bx], ax
	add bx, 2	
	
	mov word [es:bx], word KERNEL_SEGMENT
	add bx, word 2

	mov ax, KERNEL_SEGMENT
	mov es, ax
	sti
	mov sp,bp
	pop bp
	ret
	
