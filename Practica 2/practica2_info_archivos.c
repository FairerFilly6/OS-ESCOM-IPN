#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <unistd.h>      // access()                  
#include <sys/stat.h>   // stat(), struct stat        
#include <sys/types.h>  // tipos de datos del sistema 
#include <dirent.h>     // opendir(), readdir()       

#define RUTA_LEN 512

/* 
Verifica que el directorio exista antes de intentar abrirlo
Retorna  0 si existe
Retorna -1 si no existe
 */
int verificar_directorio(const char *ruta)
{
    if (access(ruta, F_OK) == -1) {
        printf("Error: el directorio '%s' no existe.\n", ruta);
        return -1;
    }
    return 0;
}

/*
Usa stat() para obtener la información de un archivo
Tamaño en bytes y fecha/hora de último acceso
Retorna  0 si stat() tuvo éxito
Retorna -1 si falló
 */
int obtener_info_archivo(const char *ruta_completa, long long *tamanio, char *fecha_acceso)
{
    struct stat info;

    //stat(): llena la estructura con información del archivo
    if (stat(ruta_completa, &info) == -1) {
        perror("stat");
        return -1;
    }

    //Tamaño en bytes
    *tamanio = info.st_size;

    //Fecha y hora de último acceso (st_atime) 
    struct tm *tiempo = localtime(&info.st_atime);
    strftime(fecha_acceso, 30, "%Y-%m-%d %H:%M:%S", tiempo);

    return 0;
}

/* 
Abre el directorio con opendir(), recorre sus entradas con readdir(),
y para cada archivo llama a obtener_info_archivo() para mostrar
su tamaño y fecha/hora de acceso.
Retorna  0 si todo salió bien
Retorna -1 si falló
 */
void listar_archivos(const char *dir)
{
    // opendir(): abre el directorio y devuelve un puntero DIR 
    DIR *directorio = opendir(dir);
    if (directorio == NULL) {
        perror("opendir");
        return;
    }

    printf("%-30s %10s  %s\n", "Archivo", "Tamaño", "Último acceso");
    printf("%-30s %10s  %s\n", "-------", "------", "-------------");

    int contador = 0;
    struct dirent *entrada;

    //readdir(): lee la siguiente entrada del directorio
    while ((entrada = readdir(directorio)) != NULL) {

        //Ignorar las entradas especiales "." y ".."
        if (strcmp(entrada->d_name, ".") == 0 ||
            strcmp(entrada->d_name, "..") == 0)
            continue;

        //Construir ruta completa para pasársela a stat()
        char ruta_completa[RUTA_LEN];
        snprintf(ruta_completa, sizeof(ruta_completa), "%s/%s", dir, entrada->d_name);

        //Obtener tamaño y fecha de acceso con stat()
        long long tamanio;
        char fecha_acceso[30];

        if (obtener_info_archivo(ruta_completa, &tamanio, fecha_acceso) == 0) {
            printf("%-30s %lld B  %s\n",
                   entrada->d_name,
                   tamanio,
                   fecha_acceso);
            contador++;
        }
    }

    //closedir(): cierra el directorio
    closedir(directorio);

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