#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>

void leerArchivo(char *nombreArchivo){

    int fd;
    char buffer[1024];
    int bytesLeidos;

    fd = open(nombreArchivo, O_RDONLY);

    if(fd == -1){
        perror("Error al abrir el archivo");
        exit(1);
    }

    while((bytesLeidos = read(fd, buffer, sizeof(buffer))) > 0){
        write(1, buffer, bytesLeidos);
    }

    close(fd);
}

int main(int argc, char *argv[]){

    if(argc != 2){
        printf("Uso: %s archivo.txt\n", argv[0]);
        return 1;
    }

    leerArchivo(argv[1]);

    return 0;
}