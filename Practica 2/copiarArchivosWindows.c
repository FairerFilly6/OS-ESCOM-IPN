#include <windows.h>
#include <stdio.h>

int main(int argc, char *argv[]) {

    if(argc < 3){
        printf("Uso: %s archivo1 archivo2 ... archivoDestino\n", argv[0]);
        return 1;
    }

    HANDLE archivoDestino;
    HANDLE archivoOrigen;

    DWORD bytesLeidos;
    DWORD bytesEscritos;

    char buffer[1024];

    // abrir archivo destino
    archivoDestino = CreateFile(
        argv[argc-1],
        GENERIC_WRITE,
        0,
        NULL,
        CREATE_ALWAYS,
        FILE_ATTRIBUTE_NORMAL,
        NULL
    );

    if(archivoDestino == INVALID_HANDLE_VALUE){
        printf("Error al crear archivo destino\n");
        return 1;
    }

    // recorrer archivos origen
    for(int i = 1; i < argc-1; i++){

        archivoOrigen = CreateFile(
            argv[i],
            GENERIC_READ,
            FILE_SHARE_READ,
            NULL,
            OPEN_EXISTING,
            FILE_ATTRIBUTE_NORMAL,
            NULL
        );

        if(archivoOrigen == INVALID_HANDLE_VALUE){
            printf("No se pudo abrir %s\n", argv[i]);
            continue;
        }

        // leer archivo
        while(ReadFile(archivoOrigen, buffer, sizeof(buffer), &bytesLeidos, NULL) && bytesLeidos > 0){

            WriteFile(archivoDestino, buffer, bytesLeidos, &bytesEscritos, NULL);

        }

        CloseHandle(archivoOrigen);

    }

    CloseHandle(archivoDestino);

    printf("Archivos copiados correctamente\n");

    return 0;
}