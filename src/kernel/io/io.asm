bits 16
section _TEXT class=CODE

global _putc
global _video_mode
global _puts

_video_mode:
	push bp
	mov bp, sp
	
	mov ah, 0x00
	mov al, [bp+4]
	int 0x10

	mov sp, bp
	pop bp
ret

_putc:
	push bp
	mov bp, sp
	
	mov bl, [bp + 4]
	mov al, [bp + 6]
	mov ah, 0x0e
	int 0x10 

	mov sp, bp
	pop bp
ret

