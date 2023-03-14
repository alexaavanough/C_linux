/**********************************************************************************************************
* Разработать приложение, организующее именованный канал между двумя процессами, не связанными отношением *
* «родитель-потомок». Обмен данными должен выполняться в обе стороны (туда-обратно).					  *
**********************************************************************************************************/

#include <unistd.h>
#include <sys/wait.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <fcntl.h>

int main() {

    char buffer[1024];
    int status, fifofd, fifofd2;
	
	/* Создание 2-х именованных каналов */
    if (mkfifo("myfifo_1", 0777) < 0) {
        perror("error on mkfifo1");
        exit(1);
    }
    if (mkfifo("myfifo_2", 0777) < 0) {
        perror("error on mkfifo2");
        exit(1);
    }
    unlink("myfifo_1");
    unlink("myfifo_2");
	
	/* Создание двух процессов без отношения родительский-дочерний */
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
    if (pid_l == 0) {
		/* Открытие именованного канала myfifo1 для записи */
        fifofd = open("myfifo1", O_WRONLY);
		
		/* Запись сообщения в myfifo1 */
        char hello[] = "Hello, second from first!";
        write(fifofd, hello, sizeof(hello));

		/* Открытие именованного канала myfifo2 для чтения */
        fifofd2 = open("myfifo2", O_RDONLY);
		
		/* Чтение сообщения из myfifo2 */
        read(fifofd2, buffer, sizeof(buffer));
        printf("1st read: %s\n", buffer);
        exit(0);
    }
	/************/

	/*** Right ***/
    if (pid_r == 0) {
		/* Открытие именованного канала myfifo1 для чтений */
        fifofd = open("myfifo1", O_RDONLY);
		
		/* Чтение сообщения из myfifo1 */
        read(fifofd, buffer, sizeof(buffer));
        printf("2nd read: %s\n", buffer);

		/* Открытие именованного канала myfifo2 для записи */
        fifofd = open("myfifo2", O_WRONLY);
		
		/* Запись сообщения в myfifo2 */
        char hello[] = "Hello, first from second!";
        write(fifofd, hello, sizeof(hello));
        exit(0);
    }
	/*************/
	
	/* Ожидание завершения обоих потомков */
    waitpid(pid_l, &status, WUNTRACED | WCONTINUED);
    waitpid(pid_r, &status, WUNTRACED | WCONTINUED);

    //wait(&status);
    return 0;
}

