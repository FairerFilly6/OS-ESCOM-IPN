#include <stdio.h>
#include <pthread.h>
#include <unistd.h>
#include <sys/wait.h>

#define HIJOSRONDA2 20
#define HIJOSRONDA3 15
#define HIJOSRONDA4 10

typedef struct {    
    int numHijo;
    int ronda;
} Hijos;


void* hijosFinales(void *arg){

    Hijos *hijo = (Hijos *) arg;

    printf("\nPráctica 6 Hilo Terminal");
    printf(
        "\nHola soy el ultimo hijo %d de la ronda %d\n",
        hijo->numHijo,
        hijo->ronda
    );

    return NULL;
}


void* generarCuartaRonda(void *arg){

    Hijos *hijo = (Hijos *) arg;

    pthread_t hilos[HIJOSRONDA4];
    Hijos hijos[HIJOSRONDA4];

    printf(
        "\nHola soy el hijo %d de la ronda %d\n",
        hijo->numHijo,
        hijo->ronda
    );

    for(int i = 0; i < HIJOSRONDA4; i++)
    {
        hijos[i].numHijo = i;
        hijos[i].ronda = 4;

        pthread_create(
            &hilos[i],
            NULL,
            hijosFinales,
            &hijos[i]
        );
    }

    for(int i = 0; i < HIJOSRONDA4; i++)
    {
        pthread_join(hilos[i], NULL);
    }

    return NULL;
}


void* generarTercerRonda(void *arg){

    Hijos *hijo = (Hijos *) arg;

    pthread_t hilos[HIJOSRONDA3];
    Hijos hijos[HIJOSRONDA3];

    printf(
        "\nHola soy el hijo %d de la ronda %d\n",
        hijo->numHijo,
        hijo->ronda
    );

    for(int i = 0; i < HIJOSRONDA3; i++)
    {
        hijos[i].numHijo = i;
        hijos[i].ronda = 3;

        pthread_create(
            &hilos[i],
            NULL,
            generarCuartaRonda,
            &hijos[i]
        );
    }

    for(int i = 0; i < HIJOSRONDA3; i++)
    {
        pthread_join(hilos[i], NULL);
    }

    return NULL;
}


void* primerHilo(void *arg){

    pthread_t hilos[HIJOSRONDA2];
    Hijos hijos[HIJOSRONDA2];

    printf("\nSoy el primer hilo!\n");

    for(int i = 0; i < HIJOSRONDA2; i++)
    {
        hijos[i].numHijo = i;
        hijos[i].ronda = 2;

        pthread_create(
            &hilos[i],
            NULL,
            generarTercerRonda,
            &hijos[i]
        );
    }

    for(int i = 0; i < HIJOSRONDA2; i++)
    {
        pthread_join(hilos[i], NULL);
    }

    return NULL;
}


void iniciarHilos(){

    pthread_t id_hilo;

    pthread_create(
        &id_hilo,
        NULL,
        primerHilo,
        NULL
    );

    pthread_join(id_hilo, NULL);
}


int main()
{
    pid_t pid = fork();

    if(pid == 0)
    {
        // proceso hijo
        iniciarHilos();
    }
    else
    {
        // proceso padre
        wait(NULL);
    }
    return 0;
}