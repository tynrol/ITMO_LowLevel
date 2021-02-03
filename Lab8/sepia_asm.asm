%define word_size 4
%define pixel_array r12
%define array_size r13
%define result r14

%macro wpss 2
    pxor xmm0, xmm0
    pxor xmm1, xmm1
    pxor xmm2, xmm2
    pinsrb xmm0, byte[pixel_array + 0], 0
    pinsrb xmm0, byte[pixel_array + 3], %1
    pinsrb xmm1, byte[pixel_array + 1], 0
    pinsrb xmm1, byte[pixel_array + 4], %1
    pinsrb xmm2, byte[pixel_array + 2], 0
    pinsrb xmm2, byte[pixel_array + 5], %1
    shufps xmm0, xmm0, %2
    shufps xmm1, xmm1, %2
    shufps xmm2, xmm2, %2
    cvtdq2ps xmm0, xmm0
    cvtdq2ps xmm1, xmm1
    cvtdq2ps xmm2, xmm2
%endmacro

%macro count_pixel 0
    mulps xmm0, xmm3
    mulps xmm1, xmm4
    mulps xmm2, xmm5
    cvtps2dq xmm0, xmm0
    cvtps2dq xmm1, xmm1
    cvtps2dq xmm2, xmm2
    addps xmm0, xmm1
    addps xmm0, xmm2
    pminsd xmm0, [rel max_values]
    pextrb byte[result    ], xmm0, 0
    pextrb byte[result + 1], xmm0, 4
    pextrb byte[result + 2], xmm0, 8
    pextrb byte[result + 3], xmm0, 12
%endmacro

align 16
c3_1: dd 0.272, 0.349, 0.393, 0.272
align 16
c2_1: dd 0.543, 0.686, 0.769, 0.543
align 16
c1_1: dd 0.131, 0.168, 0.189, 0.131

align 16
c3_2: dd 0.349, 0.393, 0.272, 0.349
align 16
c2_2: dd 0.686, 0.769, 0.543, 0.686
align 16
c1_2: dd 0.168, 0.189, 0.131, 0.168

align 16
c3_3: dd 0.393, 0.272, 0.349, 0.393
align 16
c2_3: dd 0.769, 0.543, 0.686, 0.769
align 16
c1_3: dd 0.189, 0.131, 0.168, 0.189

align 16
max_values: dd 0xFF, 0xFF, 0xFF, 0xFF

global _sepia_asm

_sepia_asm:
    push r12
    push r13
    push r14

    mov pixel_array, rdi
    mov array_size, rsi

    mov result, rdx

    test array_size, array_size
    jz .end

.loop:
    wpss 12, 0b00000000

    movaps xmm3, [rel c1_1]
    movaps xmm4, [rel c2_1]
    movaps xmm5, [rel c3_1]

    count_pixel

    add pixel_array, 3
    add result, 4

    wpss 8, 0b10100000

    movaps xmm3, [rel c1_2]
    movaps xmm4, [rel c2_2]
    movaps xmm5, [rel c3_2]

    count_pixel

    add pixel_array, 3
    add result, 4

    wpss 4, 0b01010000

    movaps xmm3, [rel c1_3]
    movaps xmm4, [rel c2_3]
    movaps xmm5, [rel c3_3]

    count_pixel


    add pixel_array, 6
    add result, 4

    sub array_size, 4
    jnz .loop
.end:
    pop r14
    pop r13
    pop r12
    ret

