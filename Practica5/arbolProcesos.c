#include <stdio.h>
#include <unistd.h>
#include <sys/wait.h>
#include <stdlib.h>

void crearHijos(int hijos, int pid, int hijosFinales){
    for ( int j = 0; j < hijos; j++)
    {
        pid = fork();
        if (pid == 0)
        {
            printf("Proceso: %d | Proceso Padre: %d \n", getpid(), getppid());        

            if (j == (hijos-1))
            {
                if (hijosFinales > 0)
                {
                    for (int i = 0; i < hijosFinales; i++)
                    {
                        pid = fork();

                        if (pid == 0)
                        {
                            printf("Proceso: %d | Proceso Padre: %d \n", getpid(), getppid());
                            exit(0);
                        }
                        
                    }

                    for (int i = 0; i < hijosFinales; i++)
                    {
                        wait(NULL);
                    }
                    
                }
                
            }
            

        }else{
            wait(NULL); 
            break;      
        }
        
    }

}

int main(int argc, char const *argv[])
{
    int pid, hijos;

    int i;
    
    printf("Padre | PID: %d | ProcesoPadre: %d\n", getpid(), getppid());

    for(i = 0; i < 5; i++) {

        pid = fork();

        if(pid == 0) {

            printf("Hijo %d | PID: %d | Padre: %d\n",
                   i + 1,
                   getpid(),
                   getppid());

            switch (i)
            {
            case 0:
                printf("Soy el hijo %d | PID: %d | Padre: %d\n", i+1, getpid(), getppid());
                crearHijos(4,pid,4);
                break;
            
            case 1:
                printf("Soy el hijo %d | PID: %d | Padre: %d\n", i+1, getpid(), getppid());
                crearHijos(3,pid,3);
                break;

            case 2:
                printf("Soy el hijo %d | PID: %d | Padre: %d\n", i+1, getpid(), getppid());
                crearHijos(1,pid,5);
                break;

            case 3:
                printf("Soy el hijo %d | PID: %d | Padre: %d\n", i+1, getpid(), getppid());
                crearHijos(3,pid,3);
                break;
            
            case 4:
                printf("Soy el hijo %d | PID: %d | Padre: %d\n", i+1, getpid(), getppid());
                crearHijos(4,pid,4);
                break;

            default:
                break;
            }



            exit(0); 
        }
    }

    
    for(i = 0; i < 4; i++) {
        wait(NULL);
    }

    
    return 0;
}
