#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>

int main()
{
	pid_t pid_r;
    pid_t pid_l = fork();
    if (pid_l < 0) 
	{
        perror("error on fork");
        return 1;
    }
	
	if (pid_l == 0)
	{
		char *shmem;
		/* Создание обласиь разделяемой памяти размером 1 Кб */
		int shmid = shmget(101, 1024, IPC_CREAT | 0777);
		if (shmid < 0)
		{
			perror("error on shmget");
			exit(1);
		}
		/* Подсоединяемся к разделяемой памяти */
		if ((shmem = (char *) shmat(shmid, 0, 0)) < 0)
		{
			perror("error on shmat");
			exit(1);
		}
		/* Запись в разделяемую память */
		strcpy(shmem, "Hello, from first!");

		sleep(6);
    
		printf("String from shared memory: %s\n", shmem);

		shmdt((void *)shmem);
		return 0;
	}
	
	if (pid_r != 0)
	{
		char *shmem;
		/* Открытие созданную процессом X область разделяемой памяти */
		int shmid = shmget(101, 1024, 0777);
		if (shmid < 0)
		{
			perror("error on shmget");
			exit(1);
		}
		/* Подсоединяемся к разделяемой памяти */
		if ((shmem = (char *) shmat(shmid, 0, 0)) < 0)
		{
			perror("error on shmat");
			exit(1);
		}
		/* Чтение разделяемой памяти */
		printf("String from shared memory: %s\n", shmem);
    
		//sleep(6);

		strcpy(shmem, "Hello, from second!");
    
		shmdt((void *)shmem);
		return 0;
	}

    wait(&status);
    return 0;
}