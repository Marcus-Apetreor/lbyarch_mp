section .data
    scalar_255: dd 255.0
    scalar_75:  dd 0.75 

section .text
bits 64
default rel
global imgCvtGrayFloatToInt

imgCvtGrayFloatToInt:

    push rbp
    push rsi
    push rdi
    mov rsi, rcx    ; rsi = f_img (float img)
    mov rdi, rdx    ; rdi = i_img (int8 img)
    mov ecx, r8d    ; ecx = size
    xor rbx, rbx    ; rbx for loop

.loop:
    cmp rbx, rcx
    jge .done

    ;float*255
    movss xmm0, dword [rsi + rbx*4]       ; xmm0 = current float value
    mulss xmm0, [rel scalar_255]

    ;eax holds truncated integer
    cvttss2si eax, xmm0


    cvtsi2ss xmm1, eax  
    subss xmm0, xmm1                    
    
    ;is f .75
    ucomiss xmm0, [rel scalar_75]      
    jne .store_result                     ; just jump if not equal

    ;if f is .75, then we check if it's even.
    test al, 1                           
    jz .store_result                      

    ;if it's odd, we +1 to round up
    inc eax                 

.store_result:
    mov [rdi + rbx], al
    inc rbx
    jmp .loop

.done:
    pop rdi
    pop rsi
    pop rbp
    ret