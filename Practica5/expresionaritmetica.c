#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>

#define SIZE 100

int operandos[SIZE];
int topOperandos = -1;
char operadores[SIZE];
int topOperadores = -1;

void pushOperando(int valor) {
    operandos[++topOperandos] = valor;
}

int popOperando() {
    return operandos[topOperandos--];
}

void pushOperador(char op) {
    operadores[++topOperadores] = op;
}

char popOperador() {
    return operadores[topOperadores--];
}

int aplicarOperacion(int a, int b, char op) {
    switch (op) {
        case '+': return a + b;
        case '-': return a - b;
        case '*': return a * b;
        case '/': return a / b;
    }
    return 0;
}

int ejecutarSistema() {
    char expresion[SIZE];
    int i;

    printf("Ingrese la expresion:\n");
    scanf("%s", expresion);

    for (i = 0; expresion[i] != '\0'; i++) {
        if (isdigit(expresion[i])) {
            pushOperando(expresion[i] - '0');
        }
        else if (expresion[i] == '+' || expresion[i] == '-' ||
                 expresion[i] == '*' || expresion[i] == '/') {
            pushOperador(expresion[i]);
        }
        else if (expresion[i] == ')') {
            int b = popOperando();
            int a = popOperando();
            char op = popOperador();
            int resultado = aplicarOperacion(a, b, op);
            pushOperando(resultado);
        }
    }

    printf("Resultado de la expresion: %d\n", popOperando());
    return 0;
}

int main(int argc, char const *argv[]) {
    ejecutarSistema();
    return 0;
}