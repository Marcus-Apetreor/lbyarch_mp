; convert.asm - Windows x64 MASM-compatible ASM (Visual Studio)
section .data
scalar_255: dd 255.0

section .text
bits 64
default rel
global imgconvert

imgconvert:
    ; Windows x64 calling convention
    ; RCX = f_img (float*)
    ; RDX = i_img (uint8_t*)
    ; R8  = size  (int)

    push rbp
    push rsi
    push rdi

    mov rsi, rcx      ; rsi = f_img
    mov rdi, rdx      ; rdi = i_img
    mov ecx, r8d      ; ecx = size (use ecx for loop)

    xor rbx, rbx      ; rbx = loop index

.loop:
    cmp rbx, rcx
    jge .done

    ; load float from [rsi + rbx*4]
    movss xmm0, dword [rsi + rbx*4]       ; load float
    movss xmm1, dword [rel scalar_255]    ; xmm1 = 255.0
    mulss xmm0, xmm1                      ; xmm0 *= 255.0
    cvtss2si eax, xmm0                    ; eax = int(round(xmm0))
    mov [rdi + rbx], al                   ; store in int buffer

    inc rbx
    jmp .loop

.done:
    pop rdi
    pop rsi
    pop rbp
    ret
