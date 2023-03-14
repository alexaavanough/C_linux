/***************************************************************************************************
* Реализовать безопасную обработку сигналов  HUP, INT, QUIT с помощью системного вызова sigaction. *
* Продемонстрировать работу скрипта вызовом команды kill.										   *
***************************************************************************************************/

#include <signal.h>
#include <unistd.h>
#include <errno.h>
#include <stdio.h>
#include <string.h>

volatile sig_atomic_t signal_number;

void handler(int signo) {
    /* signo автоматически блокируется функцией sigaction() */
    signal_number = signo;
}

int main(int argc, char **argv) {
	printf("PID = %d\n", getpid());
    sigset_t set;
    struct sigaction act;

	/* Создать пустой набор */
    sigemptyset(&set); 
	
	/* Добавить в набор SIGHUP, SIGINT и SIGQUIT */
    sigaddset(&set, SIGHUP); 
    sigaddset(&set, SIGINT); 
    sigaddset(&set, SIGQUIT); 

	/* Заблокировать SIGHUP, SIGINT и SIGQUIT */
    sigprocmask(SIG_BLOCK, &set, NULL); 
	
	/* Настроить обработчик */
    act.sa_mask = set; 
    act.sa_handler = handler;
    act.sa_flags = 0;
	
	/* Установить обработчик */
    sigaction(SIGHUP, &act, NULL); 
    sigaction(SIGINT, &act, NULL); 
    sigaction(SIGQUIT, &act, NULL);

    /* Восстановить пустой набор, который допускает SIGHUP, SIGINT и SIGQUIT */
    sigemptyset(&set);
	
    while (1) 
	{
		/* Ждать появления SIGHUP, SIGINT или SIGQUIT */
        sigsuspend(&set); 
        if (errno != EINTR) {
            perror("error on sigsuspend");
            return 1;
        }
		
        if (signal_number == SIGHUP || signal_number == SIGINT || signal_number == SIGQUIT) 
		{
            printf("signal: %s\n", strsignal(signal_number));
            /* Обработка сигнала. SIGHUP, SIGINT и SIGQUIT здесь снова блокируются */
            break;
        }
    }
	/* <любой код> */
    return 0;
}