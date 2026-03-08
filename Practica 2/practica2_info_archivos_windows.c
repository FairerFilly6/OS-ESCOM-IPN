#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <windows.h>

#define RUTA_LEN 512

/*
verificar_directorio()
Usa GetFileAttributes() para comprobar que la ruta exista
y que sea un directorio.
Retorna  0 si existe y es directorio.
Retorna -1 si no existe o no es directorio.
*/
int verificar_directorio(const char *ruta)
{
    // GetFileAttributes(): devuelve los atributos de la ruta
    DWORD atributos = GetFileAttributes(ruta);

    if (atributos == INVALID_FILE_ATTRIBUTES) {
        printf("Error: el directorio '%s' no existe.\n", ruta);
        return -1;
    }

    if (!(atributos & FILE_ATTRIBUTE_DIRECTORY)) {
        printf("Error: '%s' no es un directorio.\n", ruta);
        return -1;
    }

    return 0;
}

/*
convertir_fecha()
Convierte un FILETIME de Windows a una cadena legible.
Usa FileTimeToSystemTime() para hacer la conversión.
*/
void convertir_fecha(FILETIME ft, char *fecha_acceso)
{
    SYSTEMTIME st;

    // FileTimeToSystemTime(): convierte FILETIME → SYSTEMTIME (año, mes, dia, hora...)
    FileTimeToSystemTime(&ft, &st);

    sprintf(fecha_acceso, "%04d-%02d-%02d %02d:%02d:%02d",
            st.wYear, st.wMonth,  st.wDay,
            st.wHour, st.wMinute, st.wSecond);
}

/*
listar_archivos()
Usa FindFirstFile() y FindNextFile() para recorrer el directorio.
Por cada archivo muestra su nombre, tamaño y fecha de último acceso.
*/
void listar_archivos(const char *dir)
{
    // Construye patrón de búsqueda
    char patron[RUTA_LEN];
    sprintf(patron, "%s\\*", dir);

    WIN32_FIND_DATA datos_archivo;

    // FindFirstFile() abre el directorio y devuelve el primer archivo encontrado
    HANDLE buscador = FindFirstFile(patron, &datos_archivo);

    if (buscador == INVALID_HANDLE_VALUE) {
        printf("Error: no se pudo abrir el directorio '%s'.\n", dir);
        return;
    }

    printf("%-30s %12s  %s\n", "Archivo", "Tamanio", "Ultimo acceso");
    printf("%-30s %12s  %s\n", "-------", "-------", "-------------");

    int contador = 0;

    do {
        // Ignorar las entradas especiales "." y ".."
        if (strcmp(datos_archivo.cFileName, ".")  == 0 ||
            strcmp(datos_archivo.cFileName, "..") == 0)
            continue;


        // Calcular tamaño real combinando la parte alta y baja
        long long tamanio = ((long long)datos_archivo.nFileSizeHigh << 32)
                           | datos_archivo.nFileSizeLow;

        // Convertir fecha de último acceso
        char fecha_acceso[30];
        convertir_fecha(datos_archivo.ftLastAccessTime, fecha_acceso);

        printf("%-30s %10lld B  %s\n",
               datos_archivo.cFileName,
               tamanio,
               fecha_acceso);

        contador++;

    // FindNextFile() avanza al siguiente archivo del directorio
    } while (FindNextFile(buscador, &datos_archivo) != 0);

    // FindClose() cierra el manejador del buscador
    FindClose(buscador);

    printf("\nTotal: %d archivo(s) listado(s).\n", contador);
}

int main(int argc, char *argv[])
{
    if (argc != 2) {
        printf("Uso: %s <ruta_directorio>\n", argv[0]);
        return -1;
    }

    const char *ruta = argv[1];

    if (verificar_directorio(ruta) == -1)  return -1;

    printf("\nListado de archivos en: %s\n\n", ruta);
    listar_archivos(ruta);

    return 0;
}