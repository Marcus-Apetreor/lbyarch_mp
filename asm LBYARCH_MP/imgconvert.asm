global imgCvtGrayFloatToInt
section .text

imgCvtGrayFloatToInt:
    ; rcx = src, rdx = dst, r8d = width, r9d = height (Windows x64 calling convention)
    push rbp
    mov rbp, rsp

    mov r10d, r8d        ; width
    imul r10d, r9d       ; width * height
    xor r11d, r11d       ; i = 0

.loop:
    cmp r11d, r10d
    jge .done

    movss xmm0, [rcx + r11d*4]    ; load float pixel
    movss xmm1, dword [rel scale] ; load 255.0f
    mulss xmm0, xmm1              ; xmm0 = f * 255.0f
    addss xmm0, dword [rel half]  ; xmm0 += 0.5f
    cvttss2si eax, xmm0           ; eax = int(xmm0)
    mov [rdx + r11d], al          ; store uint8_t

    inc r11d
    jmp .loop

.done:
    pop rbp
    ret

section .data
scale: dd 255.0
half:  dd 0.5