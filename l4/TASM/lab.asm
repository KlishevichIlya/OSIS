.186                      
.model tiny
.code
org 80h
cmd_len db ?                
cmd_line db ?              
org 100h 
                   
Program:
	jmp Start

	old09h dd 0                          
	active dw 0 
	index dw 2206

	message_handler_loaded db "Handler is loaded.$"
	message_handler_removed db "Handler is removed.$"
	message_already_load db "Handler is already installed.$"
	message_wrong_param db "Wrong params. Try again.$"
	message_handler_not_load db "New keyboard handler is not installed yet.$"

New09hHandler proc    
    push ds
    push si 
    push es
    push di 
    pusha                       
    
    mov dx, cs:index
    cmp cs:active, dx
    jnz OldHandler           
    xor ax, ax
    in  al, 60h                 
	
	cmp al, 06h
	jl OldHandler

	cmp al, 0Ah
	jg OldHandler

	NewHandler:
	mov ah, al
	push ax
	in al, 61h
	or al, 10000000b
	out 61h, al
	and al, 01111111b
	out 61h, al
	
	mov ax, 40h
	mov es, ax
	pop ax
	
	sub al, 06h
	add al, 'f'
	
	mov bx, 1Ah
	mov cx, es:[bx]
	mov di, es:[bx] + 2
	cmp cx, 60
	je HighEnd
	add cx, 2
	cmp cx, di
	jz Exit
	jmp GoAhead
	
	HighEnd:
	cmp di, 30
	jz Exit
	
	GoAhead:
	mov es:[di], al
	cmp di, 60
	jnz NoWrap
	mov di, 28
	
	NoWrap:
	add di, 2
	mov es:[bx] + 2, di
	
	jmp Exit
	
	OldHandler:
	popa
    pop di 
    pop es 
    pop si 
    pop ds
    jmp dword ptr cs:old09h
	
	Exit:   
    xor ax, ax
    mov al, 20h                
    out 20h, al                
    
    popa
    pop di 
    pop es 
    pop si 
    pop ds
    
    iret                                          
New09hHandler endp

Start:    
    mov ax, 3509h              
    int 21h
    mov word ptr old09h, bx       
    mov word ptr old09h + 2, es 
	
    mov ah, cmd_len
    cmp ah, 0
    jnz ParamsRead

    mov ax, es:active
    cmp ax, index
    jz HandlerAlreadyLoad

    lea dx, message_handler_loaded
    mov ah, 09h
    int 21h

    mov ax, index
    mov active, ax

    mov ax, 2509h               
    mov dx, offset New09hHandler 
                                
    int 21h

    mov dx, offset Start       
    int 27h                    
    ret

ParamsRead:                      
    lea di, cmd_line            
    mov al, [di + 1]
    cmp al, '-'
    jnz ParamsError

    mov al, [di + 2]
    cmp al, 'r'
    jnz ParamsError

    mov ax, es:active
    cmp ax, index
    jnz HandlerNotLoad

    mov ax, 0
    mov es:active, ax

    lea dx, message_handler_removed
    mov ah, 09h
    int 21h
    ret

HandlerNotLoad:    
    lea dx, message_handler_not_load
    mov ah, 09h
    int 21h
    ret
    
ParamsError:        
    lea dx, message_wrong_param
    mov ah, 09h
	int 21h
    ret

HandlerAlreadyLoad:       
    lea dx, message_already_load
    mov ah, 09h
    int 21h
    
	ret
end Program