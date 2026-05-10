 #include <stdio.h>
 #include <dirent.h>
 #include <stdlib.h>
 #include <string.h>
 #include <time.h>
 #include <fcntl.h>       /* open, O_WRONLY, O_CREAT, O_TRUNC */
 #include <unistd.h>      /* write, close, access              */
 #include <sys/stat.h>    /* mkdir                             */
 #include <sys/types.h>



int modificarPermisos(const char *ruta, mode_t nuevos_permisos)
{
    if (chmod(ruta, nuevos_permisos) == -1) {
        perror("chmod");
        return -1;
    }

    printf("Permisos modificados correctamente para %s\n", ruta);
    return 0;
}


void mostrarInfoArchivos(struct stat info){
    printf("Tipo de archivo: ");

    if (S_ISREG(info.st_mode))
        printf("Archivo regular\n");
    else if (S_ISDIR(info.st_mode))
        printf("Directorio\n");
    else if (S_ISLNK(info.st_mode))
        printf("Enlace simbólico\n");
    else if (S_ISCHR(info.st_mode))
        printf("Dispositivo de caracteres\n");
    else if (S_ISBLK(info.st_mode))
        printf("Dispositivo de bloques\n");
    else if (S_ISFIFO(info.st_mode))
        printf("FIFO (pipe)\n");
    else if (S_ISSOCK(info.st_mode))
        printf("Socket\n");
    else
        printf("Desconocido\n");

    printf("Tamaño de archivo: %ld bytes\n", info.st_size);

    char buffer[100];
    struct tm *tiempo;

    tiempo = localtime(&info.st_atime);   // convierte time_t a struct tm
    strftime(buffer, sizeof(buffer), "%d/%m/%Y %H:%M:%S", tiempo);

    printf("Último acceso: %s\n", buffer);

}

void mostrar_permisos(mode_t mode)
{
    printf( (S_ISDIR(mode)) ? "d" : "-" );

    printf( (mode & S_IRUSR) ? "r" : "-" );
    printf( (mode & S_IWUSR) ? "w" : "-" );
    printf( (mode & S_IXUSR) ? "x" : "-" );

    printf( (mode & S_IRGRP) ? "r" : "-" );
    printf( (mode & S_IWGRP) ? "w" : "-" );
    printf( (mode & S_IXGRP) ? "x" : "-" );

    printf( (mode & S_IROTH) ? "r" : "-" );
    printf( (mode & S_IWOTH) ? "w" : "-" );
    printf( (mode & S_IXOTH) ? "x" : "-" );
}




int main(int argc, char const *argv[])
{
    char directorio[512];
    char archivo[256];
    char permisosStr[10];

    printf("Ingrese el directorio: ");
    scanf("%s", directorio);

    printf("Ingrese el nombre del archivo: ");
    scanf("%s", archivo);

    printf("Ingrese los permisos en octal (ejemplo: 755): ");
    scanf("%s", permisosStr);

    DIR *dir = opendir(directorio);

    if (!dir) {
        perror("opendir");
        return 1;
    }

    struct dirent *entry;
    struct stat info;
    char ruta[512];
    int contador = 1;

    while ((entry = readdir(dir)) != NULL) {
        if (strcmp(entry->d_name, ".") == 0 ||
            strcmp(entry->d_name, "..") == 0)
            continue;
        snprintf(ruta, sizeof(ruta), "%s/%s", directorio, entry->d_name);
        printf("\nArchivo %d: %s \n", contador, entry->d_name);

        if (stat(ruta, &info) == -1) {
            perror("stat");
            continue;
        }

        contador++;
        mostrarInfoArchivos(info);
        mostrar_permisos(info.st_mode);
        printf(" %s\n", entry->d_name);
    }

    mode_t nuevosPermisos = strtol(permisosStr, NULL, 8);
    snprintf(ruta, sizeof(ruta), "%s/%s", directorio, archivo);

    int res = modificarPermisos(ruta, nuevosPermisos);

    if (res == 1) {
        printf("Hubo un error...\n");
        perror("chmod");
    } else {
        printf("\n******************************\n");
        printf("Permisos de archivo modificados!");
        printf("\n******************************\n");
    }

    rewinddir(dir);
    contador = 1;

    while ((entry = readdir(dir)) != NULL) {
        if (strcmp(entry->d_name, ".") == 0 ||
            strcmp(entry->d_name, "..") == 0)
            continue;
        snprintf(ruta, sizeof(ruta), "%s/%s", directorio, entry->d_name);
        printf("\nArchivo %d: %s \n", contador, entry->d_name);

        if (stat(ruta, &info) == -1) {
            perror("stat");
            continue;
        }

        contador++;
        mostrarInfoArchivos(info);
        mostrar_permisos(info.st_mode);
        printf(" %s\n", entry->d_name);
    }

    closedir(dir);
    return 0;
}