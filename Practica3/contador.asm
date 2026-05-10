segment .data
    newline db 0xA

segment .bss
    handleConsola resd 1
    digito resb 1
    caractEscritos resd 1

segment .text
    global _main
    extern _GetStdHandle@4
    extern _WriteConsoleA@20
    extern _ExitProcess@4

_main:
    push dword -11
    call _GetStdHandle@4
    mov [handleConsola], eax

    mov bl, '0'

bucle:
    mov [digito], bl

    push dword 0
    push dword caractEscritos
    push dword 1
    push dword digito
    push dword [handleConsola]
    call _WriteConsoleA@20

    push dword 0
    push dword caractEscritos
    push dword 1
    push dword newline
    push dword [handleConsola]
    call _WriteConsoleA@20

    inc bl
    cmp bl, '9'+1
    jne bucle

    push dword 0
    call _ExitProcess@4