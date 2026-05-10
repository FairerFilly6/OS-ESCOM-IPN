segment .data
    msg_input  db "Ingresa una cadena: ", 0
    len_input  equ $ - msg_input
    msg_result db 10, "Longitud de la cadena: ", 0
    len_result equ $ - msg_result

segment .bss
    handleIn       resd 1
    handleOut      resd 1
    cadena         resb 101
    num_buf        resb 21
    caractLeidos   resd 1
    caractEscritos resd 1

segment .text
    global _main
    extern _GetStdHandle@4
    extern _WriteConsoleA@20
    extern _ReadConsoleA@20
    extern _ExitProcess@4

%macro imprimir 2
    push dword 0
    push dword caractEscritos
    push dword %2
    push dword %1
    push dword [handleOut]
    call _WriteConsoleA@20
%endmacro

_main:
    push dword -10
    call _GetStdHandle@4
    mov [handleIn], eax

    push dword -11
    call _GetStdHandle@4
    mov [handleOut], eax

    imprimir msg_input, len_input

    push dword 0
    push dword caractLeidos
    push dword 100
    push dword cadena
    push dword [handleIn]
    call _ReadConsoleA@20

    mov eax, [caractLeidos]
    sub eax, 2
    mov ebx, eax

    imprimir msg_result, len_result

    mov eax, ebx
    lea edi, [num_buf + 19]
    mov byte [edi], 10
    dec edi
    xor ecx, ecx
    mov esi, 10

.conv_loop:
    xor edx, edx
    div esi
    add dl, '0'
    mov [edi], dl
    dec edi
    inc ecx
    test eax, eax
    jnz .conv_loop

    inc edi
    mov edx, ecx
    inc edx

    push dword 0
    push dword caractEscritos
    push dword edx
    push dword edi
    push dword [handleOut]
    call _WriteConsoleA@20

    push dword 0
    call _ExitProcess@4