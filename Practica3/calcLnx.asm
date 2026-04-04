; ============================================================
;  Calculadora en Ensamblador x86-64 - Ubuntu 64 bits
;  Operaciones: Suma, Resta, Multiplicación, División
;  Rango de operandos: [0, 255] enteros positivos
;
;  Para compilar y ejecutar:
;    nasm -f elf64 calculadora.asm -o calculadora.o
;    ld calculadora.o -o calculadora
;    ./calculadora
; ============================================================

section .data
    ; ---- Mensajes del menú ----
    msg_titulo      db  "===================================", 0x0A
                    db  "   CALCULADORA EN ENSAMBLADOR      ", 0x0A
                    db  "===================================", 0x0A, 0
    len_titulo      equ $ - msg_titulo

    msg_menu        db  "  1. Suma       (+)", 0x0A
                    db  "  2. Resta      (-)", 0x0A
                    db  "  3. Multiplicacion (*)", 0x0A
                    db  "  4. Division   (/)", 0x0A
                    db  "  5. Salir", 0x0A
                    db  "-----------------------------------", 0x0A
                    db  "  Seleccione una opcion: ", 0
    len_menu        equ $ - msg_menu

    msg_op1         db  0x0A, "  Ingrese el primer numero  [0-255]: ", 0
    len_op1         equ $ - msg_op1

    msg_op2         db  "  Ingrese el segundo numero [0-255]: ", 0
    len_op2         equ $ - msg_op2

    msg_resultado   db  0x0A, "  Resultado: ", 0
    len_resultado   equ $ - msg_resultado

    msg_newline     db  0x0A, 0
    len_newline     equ $ - msg_newline

    msg_separador   db  "-----------------------------------", 0x0A, 0
    len_separador   equ $ - msg_separador

    msg_err_rango   db  0x0A, "  [ERROR] Numero fuera de rango [0-255].", 0x0A, 0
    len_err_rango   equ $ - msg_err_rango

    msg_err_div0    db  0x0A, "  [ERROR] Division por cero no permitida.", 0x0A, 0
    len_err_div0    equ $ - msg_err_div0

    msg_err_opcion  db  0x0A, "  [ERROR] Opcion invalida. Intente de nuevo.", 0x0A, 0
    len_err_opcion  equ $ - msg_err_opcion

    msg_adios       db  0x0A, "  Hasta luego!", 0x0A
                    db  "===================================", 0x0A, 0
    len_adios       equ $ - msg_adios

    msg_op_suma     db  " + ", 0
    msg_op_resta    db  " - ", 0
    msg_op_mul      db  " * ", 0
    msg_op_div      db  " / ", 0
    msg_op_eq       db  " = ", 0

    msg_residuo     db  "  Residuo: ", 0
    len_residuo     equ $ - msg_residuo

section .bss
    input_buf   resb 16     ; buffer para leer entrada del usuario
    num_buf     resb 32     ; buffer para convertir número a texto

section .text
    global _start

; ============================================================
;  PUNTO DE ENTRADA
; ============================================================
_start:
.loop_menu:
    ; --- Mostrar título y menú ---
    mov  rsi, msg_titulo
    call print_str

    mov  rsi, msg_menu
    call print_str

    ; --- Leer opción ---
    call read_line          ; resultado en rax (longitud), buffer en input_buf
    call parse_uint         ; convierte input_buf -> rax (valor numérico)

    cmp  rax, 1
    je   .hacer_suma
    cmp  rax, 2
    je   .hacer_resta
    cmp  rax, 3
    je   .hacer_mul
    cmp  rax, 4
    je   .hacer_div
    cmp  rax, 5
    je   .salir

    ; Opción inválida
    mov  rsi, msg_err_opcion
    call print_str
    jmp  .loop_menu

.hacer_suma:
    call leer_dos_operandos ; a -> r12,  b -> r13
    cmp  rax, 0             ; rax=0 si hubo error de rango
    je   .loop_menu
    mov  rax, r12
    add  rax, r13
    call mostrar_operacion_suma
    jmp  .loop_menu

.hacer_resta:
    call leer_dos_operandos
    cmp  rax, 0
    je   .loop_menu
    mov  rax, r12
    sub  rax, r13
    call mostrar_operacion_resta
    jmp  .loop_menu

.hacer_mul:
    call leer_dos_operandos
    cmp  rax, 0
    je   .loop_menu
    mov  rax, r12
    mul  r13                ; rax = r12 * r13  (resultado en rdx:rax)
    call mostrar_operacion_mul
    jmp  .loop_menu

.hacer_div:
    call leer_dos_operandos
    cmp  rax, 0
    je   .loop_menu
    ; Verificar divisor != 0
    cmp  r13, 0
    jne  .div_ok
    mov  rsi, msg_err_div0
    call print_str
    jmp  .loop_menu
.div_ok:
    xor  rdx, rdx           ; limpiar rdx antes de div
    mov  rax, r12
    div  r13                ; cociente -> rax, residuo -> rdx
    call mostrar_operacion_div
    jmp  .loop_menu

.salir:
    mov  rsi, msg_adios
    call print_str
    ; syscall exit(0)
    mov  rax, 60
    xor  rdi, rdi
    syscall

; ============================================================
;  leer_dos_operandos
;    Lee dos números del usuario y los valida [0,255].
;    Devuelve:  r12 = operando A
;               r13 = operando B
;               rax =  1 si OK,  0 si error de rango
; ============================================================
leer_dos_operandos:
    push rbx

    ; Pedir primer número
    mov  rsi, msg_op1
    call print_str
    call read_line
    call parse_uint
    mov  rbx, rax           ; guardar en rbx temporalmente

    ; Validar rango [0, 255]
    cmp  rbx, 255
    jg   .rango_error

    mov  r12, rbx

    ; Pedir segundo número
    mov  rsi, msg_op2
    call print_str
    call read_line
    call parse_uint
    mov  rbx, rax

    cmp  rbx, 255
    jg   .rango_error

    mov  r13, rbx
    mov  rax, 1             ; OK
    pop  rbx
    ret

.rango_error:
    mov  rsi, msg_err_rango
    call print_str
    xor  rax, rax           ; retorna 0 = error
    pop  rbx
    ret

; ============================================================
;  mostrar_operacion_* 
;    Muestra:  "  Resultado: A op B = resultado"
;    Entrada:  r12=A, r13=B, rax=resultado (rdx=residuo para div)
; ============================================================

; --- SUMA ---
mostrar_operacion_suma:
    push rdx
    push rax
    mov  rsi, msg_resultado
    call print_str
    mov  rax, r12 ; A
    call print_uint
    mov  rsi, msg_op_suma
    call print_str
    mov  rax, r13 ; B
    call print_uint
    mov  rsi, msg_op_eq
    call print_str
    pop  rax      ; resultado
    call print_uint
    mov  rsi, msg_newline
    call print_str
    mov  rsi, msg_separador
    call print_str
    pop  rdx
    ret

; --- RESTA ---
mostrar_operacion_resta:
    push rdx
    push rax
    mov  rsi, msg_resultado
    call print_str
    mov  rax, r12
    call print_uint
    mov  rsi, msg_op_resta
    call print_str
    mov  rax, r13
    call print_uint
    mov  rsi, msg_op_eq
    call print_str
    pop  rax
    call print_int          ; puede ser negativo
    mov  rsi, msg_newline
    call print_str
    mov  rsi, msg_separador
    call print_str
    pop  rdx
    ret

; --- MULTIPLICACIÓN ---
mostrar_operacion_mul:
    push rdx
    push rax
    mov  rsi, msg_resultado
    call print_str
    mov  rax, r12
    call print_uint
    mov  rsi, msg_op_mul
    call print_str
    mov  rax, r13
    call print_uint
    mov  rsi, msg_op_eq
    call print_str
    pop  rax
    call print_uint
    mov  rsi, msg_newline
    call print_str
    mov  rsi, msg_separador
    call print_str
    pop  rdx
    ret

; --- DIVISIÓN ---
mostrar_operacion_div:
    push rdx              ; rdx = residuo
    push rax              ; rax = cociente
    mov  rsi, msg_resultado
    call print_str
    mov  rax, r12
    call print_uint
    mov  rsi, msg_op_div
    call print_str
    mov  rax, r13
    call print_uint
    mov  rsi, msg_op_eq
    call print_str
    pop  rax              ; cociente
    call print_uint
    mov  rsi, msg_newline
    call print_str
    ; Mostrar residuo
    mov  rsi, msg_residuo
    call print_str
    pop  rax              ; residuo (era rdx)
    call print_uint
    mov  rsi, msg_newline
    call print_str
    mov  rsi, msg_separador
    call print_str
    ret

; ============================================================
;  read_line  — lee una línea de stdin en input_buf
;  Devuelve rax = bytes leídos
; ============================================================
read_line:
    push rdi
    push rsi
    push rdx
    mov  rax, 0             ; syscall read
    mov  rdi, 0             ; stdin
    mov  rsi, input_buf
    mov  rdx, 15
    syscall
    pop  rdx
    pop  rsi
    pop  rdi
    ret

; ============================================================
;  parse_uint — convierte string en input_buf a entero sin signo
;  Devuelve rax = valor
; ============================================================
parse_uint:
    push rbx
    push rcx
    push rsi
    mov  rsi, input_buf
    xor  rax, rax           ; acumulador = 0
    xor  rbx, rbx
.pu_loop:
    movzx rcx, byte [rsi]
    cmp  rcx, 0x0A          ; newline
    je   .pu_done
    cmp  rcx, 0x0D          ; CR
    je   .pu_done
    cmp  rcx, 0             ; fin cadena
    je   .pu_done
    cmp  rcx, '0'
    jl   .pu_skip
    cmp  rcx, '9'
    jg   .pu_skip
    sub  rcx, '0'
    imul rax, rax, 10
    add  rax, rcx
.pu_skip:
    inc  rsi
    jmp  .pu_loop
.pu_done:
    pop  rsi
    pop  rcx
    pop  rbx
    ret

; ============================================================
;  print_uint — imprime entero sin signo en rax
; ============================================================
print_uint:
    push rbx
    push rcx
    push rdx
    push rdi
    push rsi

    mov  rbx, num_buf
    add  rbx, 31            ; apuntar al final del buffer
    mov  byte [rbx], 0x0A   ; newline al final (no se imprime aquí)
    dec  rbx
    mov  rcx, 0             ; contador de dígitos

    cmp  rax, 0
    jne  .pu2_loop
    mov  byte [rbx], '0'
    dec  rbx
    inc  rcx
    jmp  .pu2_print

.pu2_loop:
    cmp  rax, 0
    je   .pu2_print
    xor  rdx, rdx
    mov  rdi, 10
    div  rdi                ; rax = cociente, rdx = dígito
    add  dl, '0'
    mov  byte [rbx], dl
    dec  rbx
    inc  rcx
    jmp  .pu2_loop

.pu2_print:
    inc  rbx                ; rbx apunta al primer dígito
    ; syscall write
    mov  rax, 1
    mov  rdi, 1
    mov  rsi, rbx
    mov  rdx, rcx
    syscall

    pop  rsi
    pop  rdi
    pop  rdx
    pop  rcx
    pop  rbx
    ret

; ============================================================
;  print_int — imprime entero con signo en rax
; ============================================================
print_int:
    cmp  rax, 0
    jge  print_uint         ; si >= 0, usar print_uint normal

    ; número negativo: imprimir '-' y negarlo
    push rax
    push rsi
    push rdx
    push rdi

    mov  rax, 1             ; write
    mov  rdi, 1
    mov  rsi, .minus
    mov  rdx, 1
    syscall

    pop  rdi
    pop  rdx
    pop  rsi
    pop  rax

    neg  rax
    jmp  print_uint

.minus: db "-"

; ============================================================
;  print_str — imprime cadena terminada en 0 apuntada por rsi
; ============================================================
print_str:
    push rax
    push rbx
    push rcx
    push rdx
    push rdi

    ; calcular longitud
    mov  rbx, rsi
    xor  rcx, rcx
.ps_len:
    cmp  byte [rbx], 0
    je   .ps_write
    inc  rbx
    inc  rcx
    jmp  .ps_len
.ps_write:
    mov  rax, 1
    mov  rdi, 1
    mov  rdx, rcx
    syscall

    pop  rdi
    pop  rdx
    pop  rcx
    pop  rbx
    pop  rax
    ret