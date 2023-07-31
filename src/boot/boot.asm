org 0x7c00

jmp main
%include "src/boot/util.asm"

reset_disk:
push dx
	mov ah, 0x00
	mov dl, [boot_disk]
	int 0x13
	mov al, ah
pop dx
ret

main:

	mov [boot_disk],dl


	xor ax,ax

	mov ss, ax
	mov ds, ax
	mov es, ax

	mov bp, 0x7c00
	mov sp, bp
	
	call reset_disk

	push str 
	call puts
	
	mov ah, 0x02
	mov al, 16 ; 32*512 bytes
	mov ch, 0
	mov cl, 2
	mov dh, 0
	mov dl, [boot_disk]
	mov bx, 0x7e00
	mov es, bx
	mov bx, 0x00

	int 0x13

	mov ax, 0x7e00
	mov es, ax
	mov ds, ax
	mov ss, ax
	mov dx, [boot_disk]

	jmp 0x7e00:0x00


jmp $


boot_disk:
	db 0

str: 
	db "Booting Raptor Kernel!",0

times 510-($-$$) db 0
dw 0xaa55

