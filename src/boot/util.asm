putc:
	push bp
	mov bp, sp

	mov ah, 0x0e
	mov al, [bp+4]
	int 0x10
	
	mov sp, bp
	pop bp
ret

puts:
	push bp
	mov bp, sp
	
	mov bx, [bp + 4]

	.loop:
		mov al, [bx]
		mov ah, 0x0e
		int 0x10

		inc bx
		cmp [bx], byte 0
		jne .loop
	
	mov sp, bp
	pop bp
ret
