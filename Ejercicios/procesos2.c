#include <stdio.h>
#include <unistd.h>
#include <sys/wait.h>

#define NHIJOS 3

void crearHijos(int hijos, int pid){
    for ( int j = 0; j < hijos; j++)
    {
        pid = fork();
        if (pid == 0)
        {
            printf("Proceso: %d | Proceso Padre: %d \n", getpid(), getppid());
             ;
        }else{
            wait(NULL);  // el padre espera a su hijo
            break;      // el padre deja de crear procesos
        }
        
    }
    // for ( int j = 0; j < hijos; j++)
    // {
    //     wait(0);
        
    // }
}


int main(int argc, char const *argv[])
{
    int pid, hijos;

    pid = fork();

    printf(" 1 Proceso inicial: %d | Proceso Padre: %d \n", getpid(), getppid());

    if (pid == 0)
    {
        pid = fork();

        

        if (pid == 0)
        {
            printf(" 2 Proceso: %d | Proceso Padre: %d \n", getpid(), getppid());
            
            pid = fork();

            if (pid == 0)
            {
                printf(" 3 Proceso: %d | Proceso Padre: %d \n", getpid(), getppid());

                for (int i = 0; i < NHIJOS; i++)
				{
					// printf("Realizando Fork a partir de %d...\n", getpid());
					pid = fork();
					if (pid == 0) //proceso hijo
					{
						printf("Proceso: %d | Proceso Padre: %d \n", getpid(), getppid());

                        switch (i)
                    {
                    case 0 :
                        hijos = 2;
                        crearHijos(hijos, pid);
                        
                        break;
                    case 1 :
                        hijos = 1;
                        crearHijos(hijos, pid);
                        break;
                    case 2 :
                        hijos = 3;
                        crearHijos(hijos, pid);
                        break;
                    default:
                        break;
                    }
                        

						return 0; //se detiene al hijo de generar otro hijo
					}
					
				}

            }


            
            wait(0);
        }
        wait(0);
    }
    

    wait(0);
    return 0;
}


