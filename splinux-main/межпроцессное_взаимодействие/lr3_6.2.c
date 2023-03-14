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