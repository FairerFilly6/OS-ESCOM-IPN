 #include <stdio.h>
 #include <stdlib.h>
 #include <string.h>
 #include <time.h>
 #include <windows.h>      
 #define MAX_ARCHIVOS  8
 #define NOMBRE_LEN    64
 #define RUTA_LEN      512
 
/* 
Devuelve el arreglo de cadenas (contenido posible de los archivos)
y escribe en *num_contenidos cuántas cadenas tiene el arreglo.
*/
 const char **get_contenidos(int *num_contenidos)
 {
     static const char *contenidos[] = {
         "Hola, soy el primer archivo creado por la practica 2.\n",
         "Los sistemas operativos administran los recursos del hardware.\n",
         "Linux usa llamadas al sistema para interactuar con el kernel.\n",
         "El sistema de archivos organiza la informacion en disco.\n",
         "La practica 2 cubre la interfaz de llamadas al sistema en Linux.\n",
     };
 
     *num_contenidos = (int)(sizeof(contenidos) / sizeof(contenidos[0]));
     return contenidos;
 }
 
 /*
Comprueba que la ruta recibida NO exista todavía
Retorna  0 si la ruta está libre
Retorna -1 si ya existe
  */
 int verificar_directorio(const char *ruta)
 {
     DWORD atributos = GetFileAttributes(ruta);

    if (atributos != INVALID_FILE_ATTRIBUTES &&
        (atributos & FILE_ATTRIBUTE_DIRECTORY)) {

        printf("Error: el directorio '%s' ya existe.\n", ruta);
        return -1;
    }

    return 0;
}
 /*
Crea el directorio con permisos 755
Retorna  0 si se creó correctamente
Retorna -1 si falló mkdir
 */
 int crear_directorio(const char *ruta)
 {
     if (CreateDirectory(ruta, NULL) == -1) {
         perror("CreateDirectory");
         return -1;
     }
     printf("Directorio creado: %s\n", ruta);
     return 0;
 }
 
 /* 
Construye la ruta completa, abre/crea el archivo, escribe el contenido y lo cierra
Retorna  0 si todo salió bien
Retorna -1 si open() falló
 */
 int crear_archivo(const char *dir, const char *nombre, const char *contenido)
 {
     char ruta_completa[RUTA_LEN];
     snprintf(ruta_completa, sizeof(ruta_completa), "%s/%s", dir, nombre);
 
     //n(): crea el archivo; O_TRUNC lo vacía si por casualidad ya existe
     HANDLE hfile = CreateFile(
        ruta_completa, 
        GENERIC_WRITE,
        0,
        NULL,
        CREATE_NEW,
        FILE_ATTRIBUTE_NORMAL,
        NULL
     );
     
    if (hfile == INVALID_HANDLE_VALUE) {
        printf("No existe o no se puede abrir\n");
    } else {
        printf("Existe\n");
        CloseHandle(hfile);
    }
    DWORD bytesEscritos;

     //write(): escribe el contenido
     WriteFile(hfile, contenido, strlen(contenido), &bytesEscritos, NULL);
 
     //close(): libera el descriptor
     CloseHandle(hfile);
 
     return 0;
 }
 
 /*
Decide cuántos archivos crear,
genera un nombre aleatorio para cada uno,
elige un contenido al azar del arreglo y llama a crear_archivo() por cada uno
 */
 void crear_archivos_aleatorios(const char *dir)
 {
     int num_contenidos;
     const char **contenidos = get_contenidos(&num_contenidos);
 
     int num_archivos = (rand() % (MAX_ARCHIVOS - 2)) + 3;
     printf("Se crearan %d archivos aleatorios.\n\n", num_archivos);
 
     for (int i = 0; i < num_archivos; i++) {
 
         //Nombre aleatorio: archivo_XXXX.txt
         char nombre[NOMBRE_LEN];
         snprintf(nombre, NOMBRE_LEN, "archivo_%04d.txt", rand() % 9000 + 1000);
 
         //Contenido aleatorio
         const char *contenido = contenidos[rand() % num_contenidos];
 
         if (crear_archivo(dir, nombre, contenido) == 0) {
             printf("  [%d] Creado: %-30s (%zu bytes)  \"%.*s...\"\n",
                    i + 1, nombre, strlen(contenido), 30, contenido);
         }
     }
 }
 
 int main(int argc, char *argv[])
 {
     if (argc != 2) {
         printf("Uso: %s <ruta_directorio>\n", argv[0]);
         return -1;
     }
 
     const char *ruta = argv[1];
 
     if (verificar_directorio(ruta) == -1)  return -1;
     if (crear_directorio(ruta)     == -1)  return -1;
 
     srand((unsigned int)time(NULL));
 
     crear_archivos_aleatorios(ruta);
 
     printf("\nTodos los archivos fueron creados en '%s'.\n", ruta);
     return 0;
 }