section .data

    msg: db "Ingresa una cadena: "
    msg_len equ $ - msg
    ;slt: db "\n"

section .bss
    resultado resb 64
    buffer resb 64
    bufferb resb 64
    bufferc resb 64
    bufferd resb 64
    buffere resb 64
    numString resb 8


section .text
global _start

_start:

    ; imprimir mensaje
    mov rdi, msg
    mov rsi, msg_len
    call print

    ;leer
    mov rdi, buffer
    mov rsi, 64
    call read_input

    ; imprimir mensaje
    mov rdi, msg
    mov rsi, msg_len
    call print

    ;leer
    mov rdi, bufferb
    mov rsi, 64
    call read_input

    ; imprimir mensaje
    mov rdi, msg
    mov rsi, msg_len
    call print

    ;leer
    mov rdi, bufferc
    mov rsi, 64
    call read_input

    ; imprimir mensaje
    mov rdi, msg
    mov rsi, msg_len
    call print

    ;leer
    mov rdi, bufferd
    mov rsi, 64
    call read_input

    ; imprimir mensaje
    mov rdi, msg
    mov rsi, msg_len
    call print

    ;leer
    mov rdi, buffere
    mov rsi, 64
    call read_input

    mov rbx, rax      ; guardar longitud real

    ;------------------------------------------------
    ;quitar saltos de linea
    ;------------------------------------------------


    mov rdi, buffer
    call remove_newline

    mov rdi, bufferb 
    call remove_newline

    mov rdi, bufferc
    call remove_newline

    mov rdi, bufferd
    call remove_newline

    mov rdi, buffere
    call remove_newline


    ;------------------------------------------------
    ;concatenacion
    ;------------------------------------------------

    mov rdi, resultado
    mov rsi, buffer
    call concat

    mov rdi, resultado
    mov rsi, bufferb
    call concat
    
    mov rdi, resultado
    mov rsi, bufferc
    call concat

    mov rdi, resultado
    mov rsi, bufferd
    call concat

    mov rdi, resultado
    mov rsi, buffere
    call concat

    ;mov rdi, resultado
    ;mov rsi, slt
    ;call concat


    ; ----------------------------
    ; Imprimir la concatenacion
    ; ----------------------------

    mov rdi, resultado
    mov rsi, 64
    call print


    ; ----------------------------
    ; Invertir la concatenacion
    ; ----------------------------

    mov rdi, resultado
    call reverse 

    ; ----------------------------
    ; Imprimir la inversion
    ; ----------------------------

    mov rdi, resultado
    mov rsi, 64
    call print

    ; ----------------------------
    ; Contar las vocales
    ; ----------------------------

    mov rdi, resultado
    call count_vowels



    ; convertir número a texto
    mov rdi, rax          ; número
    mov rsi, numString    ; buffer destino
    call itoa

    ; imprimir texto ya convertido
    mov rax, 1
    mov rdi, 1
    mov rsi, numString
    mov rdx, 8
    syscall

    ; Salir
    mov rax, 60
    xor rdi, rdi
    syscall

print:
    mov rax, 1        ; write
    mov rdx, rsi      ; len
    mov rsi, rdi      ; buffer
    mov rdi, 1        ; stdout
    syscall
    ret

read_input:
    mov rax, 0        ; read
    mov rdx, rsi
    mov rsi, rdi
    mov rdi, 0        ; stdin
    syscall
    ret

remove_newline:
    push rdi                ; Guardamos rdi (buena práctica)

.loop:
    mov al, [rdi]           ; Leer byte actual
    cmp al, 0               ; ¿Fin de cadena?
    je .end                 ; Terminamos
    cmp al, 10              ; ¿Es '\n' (ASCII 10)?
    je .replace             ; Remplazar
    inc rdi                 ; Avanza al siguiente char
    jmp .loop               ; Repetir

.replace:
    mov byte [rdi], 0       ; Reemplazamos '\n' con 0
    jmp .end

.end:
    pop rdi                 ; Restauramos rdi
    ret

; ----------------------------------------------------
; concat(destino, origen)
; RDI = destino
; RSI = origen
; ----------------------------------------------------

concat:
    push rdi        ; guardar destino original

; --- 1. Mover rdi al final de la cadena destino ---
find_end:
    mov al, [rdi]
    cmp al, 0
    je copy_src     ; si encontramos el 0, empezamos a copiar origen
    inc rdi
    jmp find_end

; --- 2. Copiar cadena origen a destino ---
copy_src:
    mov al, [rsi]
    mov [rdi], al
    inc rsi
    inc rdi
    cmp al, 0
    jne copy_src

    pop rdi         ; restaurar destino original
    ret


; --------------------------------------------------------
; reverse(cadena)
; Entrada:
;   RDI = puntero a la cadena terminada en 0
; Salida:
;   Cadena invertida (in place)
; --------------------------------------------------------

reverse:
    push rdi               ; guardar inicio original

    ; ---------------------------------
    ; 1. Buscar el final de la cadena
    ; ---------------------------------
find_end_rev:
    mov al, [rdi]
    cmp al, 0
    je end_found
    inc rdi
    jmp find_end_rev

end_found:
    dec rdi                ; retrocede al último carácter útil

    ; Ahora:
    ;   rdi = puntero al final
    ;   (valor guardado en stack) = inicio
    mov rsi, [rsp]         ; rsi = inicio

    ; ---------------------------------
    ; 2. Intercambiar start/end
    ; ---------------------------------
reverse_loop:
    cmp rsi, rdi
    jge done_reverse       ; si rsi >= rdi, terminar

    ; swap: [rsi] <-> [rdi]
    mov al, [rsi]
    mov bl, [rdi]

    mov [rsi], bl
    mov [rdi], al

    inc rsi
    dec rdi
    jmp reverse_loop

done_reverse:
    pop rdi                ; restaurar antes de salir
    ret

; ----------------------------------------------------
; count_vowels(cadena)
;   RDI = puntero a cadena terminada en 0
; Retorna:
;   RAX = número de vocales encontradas
; ----------------------------------------------------

count_vowels:
    xor rax, rax            ; rax = contador = 0

next_char:
    mov bl, [rdi]           ; obtener carácter actual
    cmp bl, 0
    je end_count            ; fin de cadena

    ; convertir en minúscula si es mayúscula:
    ; (opcional pero útil)
    cmp bl, 'A'
    jb check_vowel          ; si < 'A' no es letra
    cmp bl, 'Z'
    ja check_vowel          ; si > 'Z' no está en mayúsculas
    add bl, 32              ; convertir mayúscula → minúscula

check_vowel:
    cmp bl, 'a'
    je is_vowel
    cmp bl, 'e'
    je is_vowel
    cmp bl, 'i'
    je is_vowel
    cmp bl, 'o'
    je is_vowel
    cmp bl, 'u'
    je is_vowel

    jmp next_step

is_vowel:
    inc rax                 ; contador++

next_step:
    inc rdi                 ; siguiente carácter
    jmp next_char

end_count:
    ret

; ----------------------------------------------------
; itoa(num, buffer)
;   RDI = número a convertir
;   RSI = buffer destino (mínimo 4 bytes)
; Convierte a decimal (0-255)
; ----------------------------------------------------

itoa:
    mov rcx, 0          ; contador de dígitos
    mov rbx, 10         ; divisor

convert_loop:
    xor rdx, rdx
    div rbx             ; RAX = RAX/10, RDX = RAX % 10
    add dl, '0'         ; convertir dígito a ASCII
    push rdx            ; guardar en stack
    inc rcx             ; contar dígitos
    cmp rax, 0
    jne convert_loop

write_digits:
    pop rdx
    mov [rsi], dl       ; escribir dígito
    inc rsi
    loop write_digits

    mov byte [rsi], 0   ; terminador nulo
    ret