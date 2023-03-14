#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>

int main()
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