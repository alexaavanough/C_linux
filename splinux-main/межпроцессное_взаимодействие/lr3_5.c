/***************************************************************************************************************************
* Разработать приложение, организующее синхронизацию между двумя процессами с помощью семафоров. Реализовать две ситуации: *
* 1) проверка равенства семафора нулю и блокировкой одного из процессов;												   *
* 2) попытки уменьшить значение семафора до отрицательного и последующей блокировки;									   *
* Продемонстрировать успешную разблокировку процесса, выполнив печать сообщения в stdout.								   *
***************************************************************************************************************************/

#include <sys/ipc.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/sem.h>
#include <sys/wait.h>
#include <string.h>

int main() {
    int status;
    pid_t pid_r;
    pid_t pid_l = fork();
    if (pid_l < 0) {
        perror("error on fork");
        return 1;
    }
    if (pid_l != 0){
        pid_r = fork();
        if (pid_r < 0) {
            perror("error on fork");
            return 1;
        }
    }


	/*** Left ***/
    if (pid_l == 0) 
	{
		/* Создание или получение набора из двух семафоров */
        int semid = semget(100, 2, IPC_CREAT | 0777); 
        if (semid < 0) 
		{
            perror("Error on semget");
            exit(1);
        }
		
		/* Определение операций над семафорами */
		struct sembuf sbuf[1] = {0,	-1, SEM_UNDO }; /* Уменьшить значение 1-го семафора на 1 */

		printf("first: Now I will locked\n");

        /* Выполнение операций над семафорами */
        if (semop(semid, sbuf, 1) < 0)
        {
            perror("Error on semop");
            exit(1);
        }
		
		/*********************************************/
        /* В этом месте процесс X будет заблокирован */
		/*********************************************/
		
        printf("first: I was unlocked\n");

        exit(0);
    }
	/************/
	
	
	/*** Right ***/
	if (pid_r == 0) 
	{
		/* Создание или получение набора из двух семафоров */
        int semid = semget(100, 2, 0777);
        if (semid < 0) 
		{
            perror("error on semget");
            exit(1);
        }
		
		/* Определение операци над семафором */
        struct sembuf sbuf[1] = {0, 1, SEM_UNDO}; /* Увеличить значение семафора на 1 */
		
        printf("second: My pid is %d, and i will unlock %d after 5 seconds\n", getpid(), pid_l);

        sleep(5);
		
		/* Выполнение операции над семафором */
        if (semop(semid, sbuf, 1) < 0) 
		{
            perror("error on semop");
            exit(1);
        }
		
		/*****************************************/
        /* В этом месте процесс X разблокируется */
		/*****************************************/


        waitpid(pid_l, &status, WUNTRACED | WCONTINUED);
        auto rtrn = semctl (semid, 100, IPC_RMID, 0);
        if (rtrn == -1)
        {
            perror("Error on semctl");
            exit(1);
        }
        exit(0);
    }
	/*************/
	
	
	
	/*** Main ***/
	if (pid_r != 0 && pid_l != 0)
	{
        wait(&status);
        waitpid(pid_r, &status, WUNTRACED | WCONTINUED);
        waitpid(pid_l, &status, WUNTRACED | WCONTINUED);
        return 0;
    }
	/************/
}