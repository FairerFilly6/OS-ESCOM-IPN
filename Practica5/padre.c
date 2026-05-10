#include <windows.h>
#include <stdio.h>

int main(int argc, char *argv[])
{
    STARTUPINFO si;
    PROCESS_INFORMATION pi;

    ZeroMemory(&si, sizeof(si));
    si.cb = sizeof(si);

    ZeroMemory(&pi, sizeof(pi));

    if(argc != 2)
    {
        printf("Usar: %s Nombre_programa_hijo\n", argv[0]);
        return 1;
    }

    // Crear proceso hijo
    if(!CreateProcess(
        NULL,
        argv[1],
        NULL,
        NULL,
        FALSE,
        0,
        NULL,
        NULL,
        &si,
        &pi))
    {
        printf("Fallo al invocar CreateProcess (%d)\n", GetLastError());
        return 1;
    }

    // Proceso padre
    printf("Soy el padre\n");

    // Esperar a que termine el hijo
    WaitForSingleObject(pi.hProcess, INFINITE);

    // Cerrar handles
    CloseHandle(pi.hProcess);
    CloseHandle(pi.hThread);

    return 0;
}
