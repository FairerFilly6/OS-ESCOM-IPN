#include <stdio.h>
#include <unistd.h>
#include <sys/wait.h>

#define NHIJOS 2

int main(int argc, char const *argv[])
{
	int pid;

	pid = fork();
	// printf("Hola soy %d un nuevo proceso! \n", getpid());
	if (pid != 0)
	{
		//instrucciones del padre
		
	}else{printf("Proceso: %d | Proceso Padre: %d \n", getpid(), getppid());
		//instrucciones del hijo
		printf("Proceso: %d | Proceso Padre: %d \n", getpid(), getppid());
		printf("Realizando Fork a partir de %d...\n", getpid());

		pid = fork();
		if (pid != 0)
		{
			//instrucciones del padre
			// printf("Proceso: %d | Proceso Padre: %d \n", getpid(), getppid());
		}else{
			//instrucciones del hijo
			printf("Proceso: %d | Proceso Padre: %d \n", getpid(), getppid());
			printf("Realizando Fork a partir de %d...\n", getpid());
			pid = fork();
			if (pid != 0)
			{
				//instrucciones del padre
				// printf("Proceso: %d | Proceso Padre: %d \n", getpid(), getppid());
			}else{
				//instrucciones del hijo

				for (int i = 0; i < NHIJOS; i++)
				{
					printf("Realizando Fork a partir de %d...\n", getpid());
					pid = fork();
					if (pid == 0) //proceso hijo
					{
						printf("Proceso: %d | Proceso Padre: %d \n", getpid(), getppid());
						printf("Realizando Fork a partir de %d...\n", getpid());
						pid = fork();
						if (pid == 0)
						{
							printf("Proceso: %d | Proceso Padre: %d \n", getpid(), getppid());
							for ( i = 0; i < NHIJOS; i++)
							{	
								printf("Realizando Fork a partir de %d...\n", getpid());
								pid = fork();
								if (pid == 0)
								{
									printf("Proceso: %d | Proceso Padre: %d \n", getpid(), getppid());
									return 0;
								}
								for (int i = 0; i < NHIJOS; i++){
									wait(0);
								}
								
							}
							
							return 0;
						}
						wait(0);
						return 0; //se detiene al hijo de generar otro hijo
					}
					
				}
				
				// printf("Proceso: %d | Proceso Padre: %d \n", getpid(), getppid());

			}

		}
		


	}
	wait(0);

	wait(0);

	for (int i = 0; i < NHIJOS; i++){
		wait(0);
	}

	

	




	return 0;
}

