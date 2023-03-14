/**************************************************************************************************
* Разработать приложение, реализующее обработку сигналов HUP, INT, QUIT с помощью функции signal. *
* Приложение должно информировать о получении соответствующего сигнала вне обработчика.			  *
* Продемонстрировать работу скрипта вызовом команды kill.										  *
**************************************************************************************************/

#include <signal.h>
#include <unistd.h>
#include <stdio.h>
#include <string.h>

volatile sig_atomic_t signal_number, counter = 5;

void *handler(int signum) 
{
	/* Игнороим сигнал */
    signal(signum, SIG_IGN);
	
    /* Запоминаем номер сигнала, не тратим время на вывод */
    signal_number = signum;
	
    /* Если counter пустой, восстанавливаем обработчик по умолчанию для завершения */
    signal(signum, counter ? handler : SIG_DFL);
	
    /* Во время обработки может прийти другой сигнал! */
}

int main() 
{
    /* По умолчанию SIGQUIT заблокирован, поэтому разблокируем его*/
    sigset_t sigset;
    sigemptyset(&sigset);
    sigprocmask(SIG_SETMASK, &sigset, NULL);

	printf("PID = %d\n", getpid());
	/* Регистрируем пользовательский обрабочик для сигналов */
    signal(SIGHUP, handler); //1
    signal(SIGINT, handler); //2
    signal(SIGQUIT, handler); //3
	
	/* Ожидание 5 сигналов из списка */
    while(counter--) 
	{
        pause();
		/* Вывод перехваченых сигналов */
        printf("signal: %s\n", strsignal(signal_number));
    }
}