segment .data
    msg1      db  "Ingresa cadena1: ", 0
    len_msg1  equ $ - msg1
    msg2      db  "Ingresa cadena2: ", 0
    len_msg2  equ $ - msg2
    msg3      db  "Ingresa cadena3: ", 0
    len_msg3  equ $ - msg3
    msg4      db  "Ingresa cadena4: ", 0
    len_msg4  equ $ - msg4
    msg5      db  "Ingresa cadena5: ", 0
    len_msg5  equ $ - msg5
    msgRes    db  10, "Cadena6 (intercalada): ", 0
    len_msgRes equ $ - msgRes
    newline   db  10

segment .bss
    handleIn       resd 1
    handleOut      resd 1
    cadena1        resb 101
    cadena2        resb 101
    cadena3        resb 101
    cadena4        resb 101
    cadena5        resb 101
    cadena6        resb 510
    len1           resd 1
    len2           resd 1
    len3           resd 1
    len4           resd 1
    len5           resd 1
    lenRes         resd 1
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

%macro leer_cadena 4
    imprimir %3, %4
    push dword 0
    push dword caractLeidos
    push dword 100
    push dword %1
    push dword [handleIn]
    call _ReadConsoleA@20
    mov eax, [caractLeidos]
    sub eax, 2
    mov [%2], eax
    mov byte [%1 + eax], 0
%endmacro

_main:
    push dword -10
    call _GetStdHandle@4
    mov [handleIn], eax

    push dword -11
    call _GetStdHandle@4
    mov [handleOut], eax

    leer_cadena cadena1, len1, msg1, len_msg1
    leer_cadena cadena2, len2, msg2, len_msg2
    leer_cadena cadena3, len3, msg3, len_msg3
    leer_cadena cadena4, len4, msg4, len_msg4
    leer_cadena cadena5, len5, msg5, len_msg5

    mov ebx, [len1]
    mov eax, [len2]
    cmp eax, ebx
    jg .set_max2
    jmp .check3
.set_max2: mov ebx, eax
.check3:
    mov eax, [len3]
    cmp eax, ebx
    jg .set_max3
    jmp .check4
.set_max3: mov ebx, eax
.check4:
    mov eax, [len4]
    cmp eax, ebx
    jg .set_max4
    jmp .check5
.set_max4: mov ebx, eax
.check5:
    mov eax, [len5]
    cmp eax, ebx
    jg .set_max5
    jmp .intercalar
.set_max5: mov ebx, eax

.intercalar:
    xor edx, edx
    mov edi, cadena6

.loop_intercalar:
    cmp edx, ebx
    jge .fin_intercalar

    cmp edx, [len1]
    jge .omitir1
    mov al, [cadena1 + edx]
    mov [edi], al
    inc edi
.omitir1:
    cmp edx, [len2]
    jge .omitir2
    mov al, [cadena2 + edx]
    mov [edi], al
    inc edi
.omitir2:
    cmp edx, [len3]
    jge .omitir3
    mov al, [cadena3 + edx]
    mov [edi], al
    inc edi
.omitir3:
    cmp edx, [len4]
    jge .omitir4
    mov al, [cadena4 + edx]
    mov [edi], al
    inc edi
.omitir4:
    cmp edx, [len5]
    jge .omitir5
    mov al, [cadena5 + edx]
    mov [edi], al
    inc edi
.omitir5:

    inc edx
    jmp .loop_intercalar

.fin_intercalar:
    mov byte [edi], 0
    sub edi, cadena6
    mov [lenRes], edi

    imprimir msgRes, len_msgRes
    imprimir cadena6, [lenRes]
    imprimir newline, 1

    push dword 0
    call _ExitProcess@4