#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>
#include <pthread.h>
#include <semaphore.h>


sem_t semaphore;

void* thread_func(void* p) {
    // do circle area process here


    // call this function when the program means certain condition
    // A simple example, "counter++"
    // sem_post(&semaphore);

    pthread_exit(NULL);
}

int main(void)
{
    // reference of the variable, 0 = if want to use only on threads, initial value
    sem_init(&semaphore, 0, 0);
    for (int i = 0; i < 4; i++)
    {
        pthread_t tid;
        pthread_create(&tid, NULL, thread_func, NULL);
        pthread_detach(tid);
    }   

    // wait till other threads get executed.
    pthread_exit(0);
    sem_destroy(&semaphore);
    return 0;
}

