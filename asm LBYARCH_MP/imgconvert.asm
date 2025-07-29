global imgCvtGrayFloatToInt
section .text

imgCvtGrayFloatToInt:
    ; rcx = src, rdx = dst, r8d = width, r9d = height
    mov r10d, r8d
    imul r10d, r9d
    xor r11d, r11d
.loop:
    cmp r11d, r10d
    jge .done
    movss xmm0, [rcx + r11d*4]
    movss xmm1, dword [rel scale]
    mulss xmm0, xmm1
    addss xmm0, dword [rel half]
    cvttss2si eax, xmm0
    mov [rdx + r11d], al
    inc r11d
    jmp .loop
.done:
    ret

section .data
scale: dd 255.0
half:  dd 0.5