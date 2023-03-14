// building:  g++ -pthread lr4.cpp
#include <signal.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <pthread.h>
#include <iostream>

using namespace std;
volatile sig_atomic_t signal_number;
pthread_t Thread1, Thread2, Thread3;

void handler(int signum){
    sigset_t sigset, oldset;
    sigfillset(&sigset);

    sigprocmask(SIG_SETMASK, &sigset, &oldset);
    signal_number = signum;
    sigprocmask(SIG_SETMASK, &oldset, &sigset);

    cout << "Caught: " << sys_siglist[signal_number] << endl;
}

class Thread_HUP{
public:
    Thread_HUP(){};
    virtual ~Thread_HUP(){};
    int startThread();

protected:
    virtual void run();
private:

    static void *runThread(void *);
};

void Thread_HUP::run(){

    signal(SIGHUP, handler);
    pause();
}

int Thread_HUP::startThread(){

    int result = pthread_create(&Thread1, NULL, Thread_HUP::runThread, (void *)this);
    return result;

}

void *Thread_HUP::runThread(void *Pointer_){

    Thread_HUP *ObjectThread = (Thread_HUP *)Pointer_;
    ObjectThread->run();
}

class Thread_INT{
public:
    Thread_INT(){};
    virtual ~Thread_INT(){};
    int startThread();

protected:
    virtual void run();

private:
    static void *runThread(void *);
};

void Thread_INT::run(){
    signal(SIGINT, handler);
    pause();
}

int Thread_INT::startThread(){
    int result = pthread_create(&Thread2, NULL, Thread_INT::runThread,
                                (void *)this);
    return result;
}

void *Thread_INT::runThread(void *Pointer_){
    Thread_INT *ObjectThread =(Thread_INT *)Pointer_;
    ObjectThread->run();
}

class Thread_QUIT{
public:
    Thread_QUIT(){};
    virtual ~Thread_QUIT(){};
    int startThread();

protected:
    virtual void run();

private:
    static void *runThread(void *);
};

void Thread_QUIT::run(){
    signal(SIGQUIT, handler);
    pause();
}

int Thread_QUIT::startThread(){
    int result = pthread_create(&Thread3, NULL, Thread_QUIT::runThread, (void *)this);
    return result;
}

void *Thread_QUIT::runThread(void *Pointer_){
    Thread_QUIT *ObjectThread =(Thread_QUIT *)Pointer_;
    ObjectThread->run();
}

int main(){
    cout << getpid() << endl;

    Thread_HUP t1;
    Thread_INT t2;
    Thread_QUIT t3;

    t1.startThread();
    t2.startThread();
    t3.startThread();

    pthread_join(Thread1, NULL);
    pthread_join(Thread2, NULL);
    pthread_join(Thread3, NULL);

    return 0;
}