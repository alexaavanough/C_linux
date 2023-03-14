/*************************************************************************************************
* Разработать приложение, организующее связь между двумя процессами с помощью очереди сообщений. *
* Обмен данными должен выполняться в обе стороны (туда-обратно)								     *
* с помощью разных типов сообщений (например, 10 – туда, 20 – обратно).						     *
*************************************************************************************************/
#include <sys/ipc.h>
#include <sys/msg.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/wait.h>

int main() 
{
    pid_t child = fork();
    if (child < 0) 
	{
        perror("Error on fork");
        exit(1);
    }

    struct message_struct 
	{
        long type;
        char text[512];
    } message;
	
	/*** Child ***/
    if (child == 0) 
	{
		/* Создать или подключится к очереди сообщений */
        int msg_child = msgget(1234, IPC_CREAT | 0777);
        if (msg_child < 0) 
		{
            perror("Error on msgget");
            exit(1);
        }
		
		
		/* Отправить сообщения */
        for (int i = 0; i < 10; ++i) 
		{
			/* Сформировать сообщение */
            strcpy(message.text, "Hi, parent");
            message.type = 10;
			
			/* Отправить сообщение в очередь */
            if (msgsnd(msg_child, &message, sizeof(message.text), 0) < 0) 
			{
                perror("Error on msgsnd");
                exit(1);
            }
        }
		
		/* Принять сообщения */
        for (int i = 0; i < 10; ++i) 
		{
            if (msgrcv(msg_child, &message, sizeof(message.text), 20, 0) < 0) 
			{
                perror("Error on msgrcv");
                exit(1);
            }
            printf("Parent -> Child: %s\n", message.text);
        }
		
        exit(0);
    }
	/*************/
	
	/*** Parent ***/
	if (child != 0)
	{
		/* Создать или подключится к очереди сообщений */
        int msg_parent = msgget(1234, IPC_CREAT | 0777);
        if (msg_parent < 0) 
		{
            perror("Error on msgget");
            exit(1);
        }

		/* Отправить сообщения */
        for (int i = 0; i < 10; ++i) 
		{
			/* Сформировать сообщение */
            strcpy(message.text, "Hi, child");
            message.type = 20;
			
			/*Отправить сообщение в очередь**/
            if (msgsnd(msg_parent, &message, sizeof(message.text), 0) < 0) 
			{
                perror("Error on msgsnd");
                exit(1);
            }
        }
		
		/* Принять сообщения */
        for (int i = 0; i < 10; ++i) 
		{
            if (msgrcv(msg_parent, &message, sizeof(message.text), 10, 0) < 0) 
			{
                perror("Error on msgrcv");
                exit(1);
            }
            printf("Child -> Parent: %s\n", message.text);
        }
        sleep(2);
    }
	/**************/
	
    return 0;
}

