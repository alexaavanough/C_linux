#include <stdio.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <string.h>
#include <errno.h>
#include <fcntl.h>
#include <cstdlib>
#include <sys/stat.h>

int fdesc;
extern char **environ;
void close_fdesc() {
    close(fdesc);
    printf("Process close file!\n");
}

void my_exit() {
    printf("My exit with PID: %d\n", getpid());
}

void create_child() {
    atexit(my_exit);
    pid_t pid = fork();
    if(pid < 0) {
        fprintf(stderr,"Error on fork: %s\n", strerror(errno));
        exit(EXIT_FAILURE);
    }
    if (pid == 0) {
        printf("That's Child: PID - %d, PPID - %d\n", getpid(), getppid());
        exit(EXIT_SUCCESS);
    }
    else {
        printf("That's Parent: PID - %d, child PID %d\n", getpid(), pid);
        exit(EXIT_SUCCESS);
    }
}

void create_and_wait() {
    pid_t pid = fork();
    if(pid < 0) {
        fprintf(stderr,"Error on fork: %s\n",strerror(errno));
        exit(EXIT_FAILURE);
    }
    if (pid == 0) {
        printf("That's Child: PID - %d, PPID - %d\n", getpid(), getppid());
        sleep(60);
        exit(EXIT_SUCCESS);
    }
    else {
        printf("That's Parent: PID - %d, child PID %d\n", getpid(), pid );
        int status;
        if (waitpid(pid,&status,WUNTRACED) < 0) {
            perror("error on wait");
            exit(EXIT_FAILURE);
        }
        if (WIFEXITED(status))
            printf("Child was exited with %d\n",WEXITSTATUS(status));
        else if (WIFSTOPPED(status))
            printf("Child was stopped by %d\n", WSTOPSIG(status));
        else if (WIFSIGNALED(status))
            printf("Child was killed by %d\n", WTERMSIG(status));
    }
}

void info_of_proc() {
    pid_t pid = fork();
    int status;
    if(pid < 0) {
        perror("Error on fork");
        exit(EXIT_FAILURE);
    }
    if (pid == 0) {
        printf("That's Child: PID - %d, PPID - %d\n", getpid(), getppid());
        char str_pid[6]; sprintf(str_pid, "/%d", getpid());
        char path[27]; sprintf(path,"/proc/%s/fd", str_pid);
        char *args[] = { "ls", "-la", path, nullptr };
        execve("/bin/ls", args, environ);
        sleep(60);
    }
    wait(&status);
    sleep(60);
    if (WIFEXITED(status))
        printf("Child was exited with %d\n",WEXITSTATUS(status));
    else if (WIFSIGNALED(status))
        printf("Child was killed by %d\n", WTERMSIG(status));
    else if (WIFSTOPPED(status))
        printf("Child was stopped by %d\n", WSTOPSIG(status));
}

void process_write() {
    atexit(close_fdesc);
    pid_t pid = fork();
    if(pid < 0) {
        fprintf(stderr,"error on fork: %s\n",strerror(errno));
        exit(EXIT_FAILURE);
    }
    if (pid == 0) {
        fdesc = open("/home/user/Desktop/test.txt",O_RDWR | O_CREAT | O_TRUNC, 0666);
        if(fdesc < 0) {
            perror("Error on open\n");
            exit(EXIT_FAILURE);
        }
        char buff[33];
        sprintf(buff, "%s%d%s%d", "child pid: ", getpid(), " parent pid: ", getppid());
        write(fdesc, buff, sizeof(buff));
        sleep(30);
        exit(EXIT_SUCCESS);
    }
    else {
        printf("That's Parent: PID - %d, child PID %d\n", getpid(), pid );
        int status;
        if (waitpid(pid,&status,WUNTRACED) < 0) {
            perror("Error on wait");
            exit(1);
        }
        if (WIFEXITED(status))
            printf("Child was exited with %d\n",WEXITSTATUS(status));
        else if (WIFSIGNALED(status))
            printf("Child was killed by %d\n", WTERMSIG(status));
        else if (WIFSTOPPED(status))
            printf("Child was stopped by %d\n", WSTOPSIG(status));
    }
}

int main() {
    int choice;
    printf("Enter function for test: \n");
    printf("\t1 - Create child process\n");
    printf("\t2 - Waiting after child process\n");
    printf("\t3 - Watching information of process in /proc\n");
    printf("\t4 - Writing to file in child process\n");
    scanf("%d",&choice);
    switch (choice) {
        case 1: {
            create_child();
            break;
        }
        case 2: {
            create_and_wait();
            break;
        }
        case 3: {
            info_of_proc();
            break;
        }
        case 4: {
            process_write();
            break;
        }

        default: {
            printf("You enter bad number for testing function");
            break;
        }
    }
    return 0;
}