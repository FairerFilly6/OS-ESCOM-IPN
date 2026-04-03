; ================================================================
;  longitud.asm
;  Lee una cadena por teclado y muestra su longitud en pantalla.
; ================================================================

; ---------------------------------------------------------------
; Macro: imprimir buffer, longitud
; ----------------------------------------------------------------
%macro imprimir 2
    mov rax, 1
    mov rdi, 1
    mov rsi, %1
    mov rdx, %2
    syscall
%endmacro

section .data
    msg_input  db "Ingresa una cadena: "
    len_input  equ $ - msg_input

    msg_result db 10, "Longitud de la cadena: "
    len_result equ $ - msg_result

section .bss
    cadena   resb 101        ; buffer para la cadena (max 100 chars + null)
    num_buf  resb 21         ; buffer para el número convertido a ASCII

section .text
global _start

_start:

    ; 1. Mostrar prompt y leer cadena
    imprimir msg_input, len_input

    mov rax, 0              ; sys_read
    mov rdi, 0              ; stdin
    mov rsi, cadena         ; buffer destino
    mov rdx, 100            ; máx. bytes a leer
    syscall

    dec rax                 ; sys_read devuelve bytes leídos incluyendo '\n'
    mov r8, rax             ; decrementamos para obtener la longitud real
    ; RAX = longitud de la cadena

    ; 2. Mostrar mensaje de resultado
    imprimir msg_result, len_result
    mov rax, r8
    ; 3. Convertir longitud (RAX) a string decimal
    ;  Registros usados:
    ;    RAX = número actual (cociente en cada paso)
    ;    RDX = resto de la división (dígito)
    ;    RBX = divisor (10)
    ;    RDI = puntero que retrocede en num_buf
    ;    RCX = contador de dígitos escritos

    lea rdi, [num_buf + 20]   ; apuntar al final del buffer
    mov byte [rdi], 10        ; colocar '\n' al final
    dec rdi                   ; retroceder una posición

    xor rcx, rcx              ; rcx = 0  (contador de dígitos)
    mov rbx, 10               ; divisor fijo = 10

.conv_loop:
    xor rdx, rdx              ; limpiar RDX antes de dividir
    div rbx                   ; RAX = RAX / 10
                              ; RDX = RAX % 10

    add dl, '0'               ; convertir 0-9 a carácter ASCII '0'-'9'
    mov [rdi], dl             ; guardar el dígito en el buffer
    dec rdi                   ; retroceder
    inc rcx                   ; un dígito más contado

    test rax, rax             ; ¿el cociente llegó a 0?
    jnz .conv_loop            ; no → seguir dividiendo

    ; Después del bucle:
    ;   RDI apunta UNA posición ANTES del primer dígito
    inc rdi                   ; ahora sí apunta al primer dígito

    ; 4. Imprimir el número convertido
    mov rsi, rdi              ; fuente = primer dígito
    mov rdx, rcx
    inc rdx                   ; longitud = dígitos + 1 ('\n')

    mov rax, 1                ; sys_write
    mov rdi, 1                ; stdout
    syscall

    ; 5. Salir
    mov rax, 60
    xor rdi, rdi
    syscall
