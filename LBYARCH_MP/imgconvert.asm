section.text
section .data
    float_255: dd 255.0
    float_0_5: dd 0.5
global imgCvtGrayFloatToInt

; void imgCvtGrayFloatToInt(const float* src, uint8_t* dst, int width, int height)
imgCvtGrayFloatToInt:
    ; rcx = src, rdx = dst, r8 = width, r9 = height
    push rbp
    mov rbp, rsp

    mov rsi, rcx        ; src pointer
    mov rdi, rdx        ; dst pointer
    mov eax, r8d        ; width
    imul eax, r9d       ; eax = width * height
    mov ecx, 0          ; pixel index

    movss xmm1, dword [rel float_255]   ; xmm1 = 255.0f
    movss xmm2, dword [rel float_0_5]   ; xmm2 = 0.5f

.loop:
    cmp ecx, eax
    jge .done

    movss xmm0, dword [rsi + rcx*4]     ; xmm0 = src[ecx]
    mulss xmm0, xmm1                    ; xmm0 *= 255.0f
    addss xmm0, xmm2                    ; xmm0 += 0.5f
    cvttss2si edx, xmm0                 ; edx = (int)xmm0
    mov dl, dl                          ; zero-extend to 8 bits
    mov [rdi + ecx], dl                 ; dst[ecx] = (uint8_t)edx

    inc ecx
    jmp .loop

.done:
    pop rbp
    ret