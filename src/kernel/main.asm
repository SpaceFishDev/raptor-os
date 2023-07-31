bits 16
section _ENTRY class=CODE
global entry
extern _main_
extern safe_point
entry:

mov ax, 0x7e00
mov es,ax 

push dx
call _main_

cli
hlt

global _set_safepoint
_set_safepoint:
	push bp
	mov bp, sp
	mov ax, [bp+4]
	mov [bx], ax	
	mov sp, bp
	pop bp
ret

global _jump
_jump:
	push bp
	mov bp,sp
	mov bx, [bp+4]
	jmp bx

heap_addr:
	dw 0
	dw 0

global _get_heap
_get_heap:
	push bp
	mov bp, sp
	
	push bx
	mov ax, 8192 
	mov bx, 0x7e00 
	mov [heap_addr],bx 
	mov [heap_addr+2],ax
	mov eax, [heap_addr]
	pop bx

	mov sp, bp
	pop bp
ret
global _div64
_div64:
    push bp            
    mov bp, sp          
    push bx
    mov eax, [bp + 8]  
    mov ecx, [bp + 12] 
    xor edx, edx
    div ecx            
    mov bx, [bp + 16]
    mov [bx + 4], eax
    mov eax, [bp + 4]   
    div ecx
    mov [bx], eax
    mov bx, [bp + 18]
    mov [bx], edx
    pop bx
    mov sp, bp
    pop bp
ret

;short drive
;short c
;short s
;short h
;short sectors
;char* data

global _reset_disk
_reset_disk:
    push bp
    mov bp, sp

    mov ah, 0x00
    mov dl, [bp + 4]

    int 13h

    mov sp, bp
    pop bp
ret

global _write_sect
_write_sect:

    push bp           
    mov bp, sp          


    push bx
    push es
    mov dl, [bp + 4]  
    mov ch, [bp + 6]   
    mov cl, [bp + 7]    
    shl cl, 6
    mov al, [bp + 8]    
    and al, 3Fh
    or cl, al
    mov dh, [bp + 10]      
    mov al, [bp + 12]
    mov bx, [bp + 16]   
    mov es, bx
    mov bx, [bp + 14]
    mov ah, 03h
    stc
    int 13h
    mov ax, 1
    sbb ax, 0              
    pop es
    pop bx
    mov sp, bp
    pop bp
ret

global _read_sect
_read_sect:
    push bp           
    mov bp, sp          

    
    push bx
    push es
    mov dl, [bp + 4]  
    mov ch, [bp + 6]   
    mov cl, [bp + 7]    
    shl cl, 6
    mov al, [bp + 8]    
    and al, 3Fh
    or cl, al
    mov dh, [bp + 10]      
    mov al, [bp + 12]
    mov bx, [bp + 16]   
    mov es, bx
    mov bx, [bp + 14]
    mov ah, 02h
    stc
    int 13h
    mov ax, 1
    sbb ax, 0              
    pop es
    pop bx
    mov sp, bp
    pop bp
ret

global _outb
_outb:
    push bp
    mov bp, sp
    mov dx, [bp + 4]
    mov al, [bp + 6]
    out dx, al
    mov sp, bp
    pop bp
ret

global _inb
_inb:
    push bp
    mov bp, sp
    mov dx, [bp + 4] 
    in ax, dx
    mov sp, bp
    pop bp
ret
