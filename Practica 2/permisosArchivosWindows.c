#include <windows.h>
#include <stdio.h>

void modificarPermiso(char *rutaArchivo, char *permiso){

    DWORD atributo;

    if(strcmp(permiso, "readonly") == 0)
        atributo = FILE_ATTRIBUTE_READONLY;

    else if(strcmp(permiso, "hidden") == 0)
        atributo = FILE_ATTRIBUTE_HIDDEN;

    else if(strcmp(permiso, "system") == 0)
        atributo = FILE_ATTRIBUTE_SYSTEM;

    else if(strcmp(permiso, "normal") == 0)
        atributo = FILE_ATTRIBUTE_NORMAL;

    else{
        printf("Permiso no valido\n");
        return;
    }

    if(SetFileAttributes(rutaArchivo, atributo) == 0){
        printf("Error al cambiar permisos\n");
    }else{
        printf("Permiso modificado correctamente\n");
    }
}

void mostrarPermisos(DWORD atributos){

    printf("Permisos: ");

    if(atributos & FILE_ATTRIBUTE_READONLY)
        printf("READ_ONLY ");

    if(atributos & FILE_ATTRIBUTE_HIDDEN)
        printf("HIDDEN ");

    if(atributos & FILE_ATTRIBUTE_SYSTEM)
        printf("SYSTEM ");

    if(atributos & FILE_ATTRIBUTE_ARCHIVE)
        printf("ARCHIVE ");

    printf("\n");
}

int main(int argc, char *argv[]) {

    if(argc != 4){
        printf("Uso: %s directorio archivo permiso\n", argv[0]);
        return 1;
    }

    char rutaBusqueda[260];

    sprintf(rutaBusqueda, "%s\\*.*", argv[1]);

    WIN32_FIND_DATA datosArchivo;

    HANDLE hFind = FindFirstFile(rutaBusqueda, &datosArchivo);

    if(hFind == INVALID_HANDLE_VALUE){
        printf("No se pudo abrir el directorio\n");
        return 1;
    }

    do {

        printf("\nNombre: %s\n", datosArchivo.cFileName);

        printf("Size: %lu bytes\n", datosArchivo.nFileSizeLow);

        FILETIME ft = datosArchivo.ftLastAccessTime;

        SYSTEMTIME st;

        FileTimeToSystemTime(&ft, &st);

        printf("Ultimo acceso: %02d/%02d/%d %02d:%02d\n",
            st.wDay,
            st.wMonth,
            st.wYear,
            st.wHour,
            st.wMinute
        );

        mostrarPermisos(datosArchivo.dwFileAttributes);

        

    } while(FindNextFile(hFind, &datosArchivo));

    FindClose(hFind);

    modificarPermiso(argv[2], argv[3]);

     hFind = FindFirstFile(rutaBusqueda, &datosArchivo);
    
    do {

        printf("\nNombre: %s\n", datosArchivo.cFileName);

        printf("Size: %lu bytes\n", datosArchivo.nFileSizeLow);

        FILETIME ft = datosArchivo.ftLastAccessTime;

        SYSTEMTIME st;

        FileTimeToSystemTime(&ft, &st);

        printf("Ultimo acceso: %02d/%02d/%d %02d:%02d\n",
            st.wDay,
            st.wMonth,
            st.wYear,
            st.wHour,
            st.wMinute
        );

        mostrarPermisos(datosArchivo.dwFileAttributes);

        

    } while(FindNextFile(hFind, &datosArchivo));




    FindClose(hFind);

    return 0;
}