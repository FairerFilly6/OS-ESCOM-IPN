#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>

int main(int argc, char *argv[]) {

    if(argc < 3){
        printf("Uso: %s archivo1 archivo2 ... archivoDestino\n", argv[0]);
        return 1;
    }

    int fdDestino;
    int fdOrigen;
    char buffer[1024];
    int bytes;

    // el último argumento es el archivo destino
    fdDestino = open(argv[argc-1], O_WRONLY | O_CREAT | O_TRUNC, 0644);

    if(fdDestino == -1){
        perror("Error al abrir archivo destino");
        exit(1);
    }

    // recorrer archivos origen
    for(int i = 1; i < argc-1; i++){

        fdOrigen = open(argv[i], O_RDONLY);

        if(fdOrigen == -1){
            perror("Error al abrir archivo origen");
            continue;
        }

        while((bytes = read(fdOrigen, buffer, sizeof(buffer))) > 0){
            write(fdDestino, buffer, bytes);
        }

        close(fdOrigen);
    }

    close(fdDestino);

    return 0;
}