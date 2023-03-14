/***********************************************************************************
* Разработать приложение, организующее неименованный канал между двумя процессами. *
* Обмен данными должен выполняться в обе стороны (туда-обратно).				   *
***********************************************************************************/

#include <unistd.h>
#include <sys/wait.h>
#include <stdio.h>
#include <stdlib.h>

int main() {
    int pipe_lr[2], pipe_rl[2], status;
	
	/* Создание 2 неименованных каналов: L->R, R->L */
    if (pipe(pipe_lr) < 0) 
	{
        perror("Error on pipe");
        exit(1);
    }
    if (pipe(pipe_rl) < 0) 
	{
        perror("Error on pipe");
        exit(1);
    }
	
	/* Создание двух процессов */
    pid_t pid_l = fork();
    if (pid_l == -1) 
	{
        perror("Error on fork");
        return 1;
    }
	
    char buff_l[1024];
    char buff_r[1024];
	
	/*** Left ***/
    if (pid_l == 0) 
	{
        close(pipe_lr[0]); /* Закрытие "конца" для чтения канала LR */
		close(pipe_rl[1]); /* Закрытие "конца" для записи канала RL */
		
		/* Запись сообщения в канал */
        char hello_sec[] = "Hello, second from first!";
        write(pipe_lr[1], hello_sec, sizeof(hello_sec));
		
		/* Чтение сообщения из канала */
        read(pipe_rl[0], buff_l, sizeof(buff_l));
        printf("1st read: %s\n", buff_l);
		
        exit(0);
    }
	/************/
	
	/*** Right ***/
	if (pid_l != 0)
	{
        close(pipe_lr[1]); /* Закрытие "конца" для записи канала LR */
		close(pipe_rl[0]); /* Закрытие "конца" для чтения канала RL */
		
		/* Запись сообщения в канал */
        char hello_first[] = "Hello, first from second!";
        write(pipe_rl[1], hello_first, sizeof(hello_first));
		
		/* Чтение сообщения из канала */
        read(pipe_lr[0], buff_r, sizeof(buff_r));
        printf("2nd read: %s\n", buff_r);

        waitpid(pid_l, &status, WUNTRACED | WCONTINUED);
        return 0;
    }
	/*************/
}