/****************************************************************************************************
* Разработать приложение, реализующее обработку сигналов HUP, INT, QUIT с помощью сигнальной маски. *
* Продемонстрировать работу скрипта вызовом команды kill.											*
****************************************************************************************************/

#include <signal.h>
#include <unistd.h>
#include <stdio.h>
#include <string.h>

volatile sig_atomic_t signal_number, counter = 5;

void *handler(int signum) {
    sigset_t sigset, oldset;
	
	/* Полностью инициализируем набор sigset, в котором содержатся все сигналы */
    sigfillset(&sigset);
	
	/* Устанавливаем новую сигнальную маску указанную в sigset, при этом запоминая старую */
    sigprocmask(SIG_SETMASK, &sigset, &oldset);
	
	/* Запоминаем полученый сигнал */
    signal_number = signum;
	
	/* Устанавливаем старую маску для завершения */
    sigprocmask(SIG_SETMASK, &oldset, &sigset);
}

int main() {
	printf("PID = %d\n", getpid());
    signal(SIGINT, handler);
    signal(SIGHUP, handler);
    signal(SIGQUIT, handler);
	
    while(counter--) 
	{
        pause();
        printf("signal: %s\n", strsignal(signal_number));
    }
}