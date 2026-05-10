#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <stdlib.h>

int main()
{
    pid_t pid_hijo, pid_nieto1, pid_nieto2, pid_nieto3;
    char *args1[] = {"./expresionaritmetica", NULL};
    char *args2[] = {"./permisosarchivos" NULL};
    char *args3[] = {"./ProcesosSecLnx", NULL};

    pid_hijo = fork();

    if (pid_hijo == 0)
    {
        /* Proceso hijo: crea tres nietos */
        printf("Soy el hijo, PID: %d, mi padre es: %d\n", getpid(), getppid());

        pid_nieto1 = fork();
        if (pid_nieto1 == 0)
        {
            printf("Soy el hijo 1, PID: %d, mi padre es: %d, ejecutando programa de expresiones aritméticas\n", getpid(), getppid());
            execv("./expresionaritmetica", args1);
            exit(0);
        }
        wait(NULL); // Espera a que el primer nieto termine

        pid_nieto2 = fork();
        if (pid_nieto2 == 0)
        {
            printf("Soy el hijo 2, PID: %d, mi padre es: %d, ejecutando programa de permisos de archivos\n", getpid(), getppid());
            execv("./permisosarchivos", args2);
            exit(0);
        }
        wait(NULL); // Espera a que el segundo nieto termine

        pid_nieto3 = fork();
        if (pid_nieto3 == 0)
        {
            printf("Soy el hijo 3, PID: %d, mi padre es: %d, ejecutando programa de procesos secuenciales\n", getpid(), getppid());
            execv("./ProcesosSecLnx", args3);
            exit(0);
        }
        wait(NULL); // Espera a que el tercer nieto termine

        exit(0);
    }
    else
    {
        /* Proceso padre espera al hijo */
        printf("Soy el padre, PID: %d\n", getpid());
        wait(NULL);
        printf("El padre termina\n");
        exit(0);
    }
}