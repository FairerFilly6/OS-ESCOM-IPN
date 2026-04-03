section .data
    cadena db 'Programando en ensamblador para Linux', 0xA
    longitud equ $ - cadena    ; Calcula la longitud automáticamente

section .text
    global _start

_start:
    ; sys_write(1, cadena, longitud)
    mov     rax, 1          ; syscall write = 1
    mov     rdi, 1          ; file descriptor 1 = stdout
    mov     rsi, cadena     ; dirección de la cadena
    mov     rdx, longitud   ; longitud de la cadena
    syscall

    ; sys_exit(0)
    mov     rax, 60         ; syscall exit = 60
    xor     rdi, rdi        ; código de salida 0
    syscall
