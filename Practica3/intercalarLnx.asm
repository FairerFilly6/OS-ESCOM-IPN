; ================================================================
;  intercalar.asm
;  Copia 5 cadenas (ingresadas por teclado) a cadena6
;  intercalando sus caracteres posición a posición.
;
;  Compilar:  nasm -f elf64 intercalar.asm -o intercalar.o
;  Enlazar:   ld intercalar.o -o intercalar
;  Ejecutar:  ./intercalar
;
;  Registros índice principales:
;    RSI  (fuente / Source Index)  ← base de cada cadenaX
;    RDI  (destino / Dest. Index)  ← puntero de escritura en cadena6
;    R8   índice de columna i
;    R9   longitud máxima (max de los 5 lengths)
;    RCX  longitud de la cadena actual en el chequeo
; ================================================================

section .data
    msg1      db  "Ingresa cadena1: "
    len_msg1  equ $ - msg1
    msg2      db  "Ingresa cadena2: "
    len_msg2  equ $ - msg2
    msg3      db  "Ingresa cadena3: "
    len_msg3  equ $ - msg3
    msg4      db  "Ingresa cadena4: "
    len_msg4  equ $ - msg4
    msg5      db  "Ingresa cadena5: "
    len_msg5  equ $ - msg5
    msgRes    db  10, "Cadena6 (intercalada): "
    len_msgRes equ $ - msgRes
    newline   db  10

section .bss
    cadena1   resb 101
    cadena2   resb 101
    cadena3   resb 101
    cadena4   resb 101
    cadena5   resb 101
    cadena6   resb 510
    len1      resq 1
    len2      resq 1
    len3      resq 1
    len4      resq 1
    len5      resq 1
    lenRes    resq 1

section .text
global _start

; ------------------------------------------------------------------
; Macro para imprimir (write syscall)
;   %1 = dirección del buffer
;   %2 = número de bytes
; ------------------------------------------------------------------
%macro imprimir 2
    mov  rax, 1
    mov  rdi, 1
    mov  rsi, %1
    mov  rdx, %2
    syscall
%endmacro

; ------------------------------------------------------------------
; Macro para leer cadena desde stdin y guardar longitud
;   %1 = buffer destino
;   %2 = variable qword donde guardar la longitud
;   %3 = prompt a imprimir
;   %4 = longitud del prompt
; ------------------------------------------------------------------
%macro leer_cadena 4
    imprimir %3, %4         ; mostrar prompt

    mov  rax, 0             ; sys_read
    mov  rdi, 0             ; stdin
    mov  rsi, %1            ; buffer
    mov  rdx, 100           ; máx 100 bytes
    syscall

    dec  rax                        ; descontar '\n'
    mov  [%2], rax                  ; guardar longitud real
    mov  byte [%1 + rax], 0         ; colocar terminador null
%endmacro

_start:

    ;  1. LEER LAS 5 CADENAS
    leer_cadena cadena1, len1, msg1, len_msg1
    leer_cadena cadena2, len2, msg2, len_msg2
    leer_cadena cadena3, len3, msg3, len_msg3
    leer_cadena cadena4, len4, msg4, len_msg4
    leer_cadena cadena5, len5, msg5, len_msg5

    ;  2. CALCULAR LA LONGITUD MÁXIMA ENTRE LAS 5 CADENAS
    mov  r9, [len1]         ; R9 = max_len  (inicia con len1)

    mov  rax, [len2]
    cmp  rax, r9
    cmovg r9, rax           ; si len2 > R9  →  R9 = len2

    mov  rax, [len3]
    cmp  rax, r9
    cmovg r9, rax

    mov  rax, [len4]
    cmp  rax, r9
    cmovg r9, rax

    mov  rax, [len5]
    cmp  rax, r9
    cmovg r9, rax

    ;  3. INTERCALAR
    ;  RSI ← base de la cadena fuente
    ;  RDI ← puntero de escritura en cadena6
    ;  R8  ← índice de columna i
    ;==============================================================
    xor  r8,  r8            ; R8  = i = 0
    mov  rdi, cadena6       ; RDI apunta al inicio de cadena6

.loop_intercalar:
    cmp  r8, r9
    jge  .fin_intercalar    ; si i >= max_len → terminar

    ;--- cadena1[i] ---
    mov  rcx, [len1]
    cmp  r8, rcx
    jge  .omitir1
    mov  rsi, cadena1       ; RSI = &cadena1[0]
    mov  al,  [rsi + r8]    ; AL  = cadena1[i]
    mov  [rdi], al          ; cadena6[dest] = cadena1[i]
    inc  rdi                ; dest++
.omitir1:

    ;--- cadena2[i] ---
    mov  rcx, [len2]
    cmp  r8, rcx
    jge  .omitir2
    mov  rsi, cadena2
    mov  al,  [rsi + r8]
    mov  [rdi], al
    inc  rdi
.omitir2:

    ;--- cadena3[i] ---
    mov  rcx, [len3]
    cmp  r8, rcx
    jge  .omitir3
    mov  rsi, cadena3
    mov  al,  [rsi + r8]
    mov  [rdi], al
    inc  rdi
.omitir3:

    ;--- cadena4[i] ---
    mov  rcx, [len4]
    cmp  r8, rcx
    jge  .omitir4
    mov  rsi, cadena4
    mov  al,  [rsi + r8]
    mov  [rdi], al
    inc  rdi
.omitir4:

    ;--- cadena5[i] ---
    mov  rcx, [len5]
    cmp  r8, rcx
    jge  .omitir5
    mov  rsi, cadena5
    mov  al,  [rsi + r8]
    mov  [rdi], al
    inc  rdi
.omitir5:

    inc  r8
    jmp  .loop_intercalar

.fin_intercalar:
    mov  byte [rdi], 0      ; null-terminar cadena6

    ;--- calcular longitud final de cadena6 ---
    mov  rax, cadena6
    sub  rdi, rax           ; RDI = número de bytes escritos
    mov  [lenRes], rdi

    ;  4. MOSTRAR cadena6
    imprimir msgRes,   len_msgRes
    imprimir cadena6,  [lenRes]
    imprimir newline,  1

    ;  5. SALIDA
    mov  rax, 60
    xor  rdi, rdi
    syscall
