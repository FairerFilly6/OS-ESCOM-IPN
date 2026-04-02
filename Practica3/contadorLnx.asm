section .data
    newline db 0xA              ; Salto de línea '\n'

section .bss
    digito resb 1               ; 1 byte reservado para el dígito actual

section .text
    global _start

_start:
    mov bl, '0'                 ; BL = '0' (ASCII 48) — inicio del contador

bucle:
    mov [digito], bl            ; Guarda BL en memoria (sys_write necesita dirección)

    mov rax, 1                  ; syscall número 1 = sys_write
    mov rdi, 1                  ; rdi = file descriptor 1 (stdout)
    mov rsi, digito             ; rsi = dirección del dato a imprimir
    mov rdx, 1                  ; rdx = número de bytes a escribir
    syscall                     ; Interrupción 64 bits — llama al kernel

    mov rax, 1                  ; sys_write de nuevo
    mov rdi, 1                  ; stdout
    mov rsi, newline            ; dirección del salto de línea
    mov rdx, 1                  ; 1 byte
    syscall                     ; Imprime '\n'

    inc bl                      ; BL++ → siguiente carácter ASCII

    cmp bl, '9'+1               ; ¿BL llegó a ':' (ASCII 58)?
    jne bucle                   ; Si NO → regresa al bucle
                                ; Si SÍ → continúa hacia abajo

    mov rax, 60                 ; syscall número 60 = sys_exit
    mov rdi, 0                  ; código de salida 0 (éxito)
    syscall                     ; Termina el programa