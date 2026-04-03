section .bss
cadena resb 50          ; Espacio para la cadena

section .text
global _start

_start:
    ; sys_read (rax=0)
    mov rax, 0          ; syscall: read
    mov rdi, 0          ; stdin
    mov rsi, cadena     ; buffer
    mov rdx, 50         ; tamaño
    syscall

    ; sys_write (rax=1)
    mov rax, 1          ; syscall: write
    mov rdi, 1          ; stdout
    mov rsi, cadena     ; buffer
    mov rdx, 50         ; tamaño
    syscall

    ; sys_exit (rax=60)
    mov rax, 60         ; syscall: exit
    xor rdi, rdi        ; código de salida 0
    syscall
