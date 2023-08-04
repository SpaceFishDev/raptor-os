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
    mov ax, 0x7e00
    mov es, ax
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
global _init_pit
_init_pit:
    push bp
    mov bp, sp
    out 0x43, al                
    mov ax, 11931               
    out 0x40, al                
    mov al, ah                  
    out 0x40, al               
    mov sp, bp
    pop bp
ret

global _init_pic
_init_pic:
    mov al, 0x11                ; load the ICW1 into al
    out 0x20, al                ; send it to the port 0x20(master PIC)
    out 0xa0, al                ; send it to the port 0xa0(slave PIC)
    mov al, 0x20                ; the number of the 0 interrupt of the master PIC
    out 0x20, al                ; send it to the master PIC. Now IRQ0 is interrupt 0x20 in the IVT table
    mov al, 0x28                ; the number of the first interrupt of the slave PIC
    out 0xa0, al                ; send it to the slave PIC. Now IRQ9 is interrupt 0x28 in the IVT table
    mov al, 0x4                 ; 0x4 - IRQ2, used to call the slave PIC
    out 0x21, al                ; send it to the master PIC
    mov al, 0x2                 ; IRQ2 for the slacve PIC
    out 0xa1, al                ; send it to the slave PIC
    mov al, 1                   ; only the bit 0 is set. 
    out 0x21, al                ; by sending it, tell the master PIC, that we are in a 8086 PC
    out 0xa1, al                ; same, for the slave PIC
    xor al, al
    out 0x21, al
    out 0xa1, al
ret

global __U4D ; needed for watcom again...
__U4D:
    shl edx, 16         
    mov dx, ax         
    mov eax, edx      
    xor edx, edx
    shl ecx, 16       
    mov cx, bx   
    div ecx     
    mov ebx, edx
    mov ecx, edx
    shr ecx, 16
    mov edx, eax
    shr edx, 16
ret

global __U4M
__U4M: ; this is needed for WATCOM to function
    shl edx, 16       
    mov dx, ax       
    mov eax, edx   
    shl ecx, 16         
    mov cx, bx          
    mul ecx             
    mov edx, eax      
    shr edx, 16
ret

global _test_func
_test_func:
    mov ah, 0x02
    mov dl, 'a'
    int 21h
ret

extern _int21H
global _int21_handler_stage0
extern _printf

_int21_handler_stage0:
    push ax
    call _int21H 
    push ax
    mov al, 0x20
    out 0x20, al
    pop ax
    add sp, 2
    iret

global _call
_call:
	push bp
	mov bp,sp
	mov bx, [bp+4]
    mov ax, 0x7e00
    mov es, ax
    call bx
    mov sp, bp
    pop bp
ret

_str:
    db `test: %c\n`,0

