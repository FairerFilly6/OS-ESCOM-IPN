#include <windows.h>
#include <stdio.h>

int main(int argc, char *argv[]) {

    if(argc != 2){
        printf("Uso: %s archivo\n", argv[0]);
        return 1;
    }

    HANDLE archivo;
    DWORD bytesLeidos;

    char buffer[1024];

    archivo = CreateFile(
        argv[1],            // nombre del archivo
        GENERIC_READ,       // permiso de lectura
        0,                  // no compartir
        NULL,
        OPEN_EXISTING,      // abrir archivo existente
        FILE_ATTRIBUTE_NORMAL,
        NULL
    );

    if(archivo == INVALID_HANDLE_VALUE){
        printf("Error al abrir archivo\n");
        return 1;
    }

    while(ReadFile(archivo, buffer, sizeof(buffer)-1, &bytesLeidos, NULL) && bytesLeidos > 0){

        buffer[bytesLeidos] = '\0';

        printf("%s", buffer);
    }

    CloseHandle(archivo);

    return 0;
}