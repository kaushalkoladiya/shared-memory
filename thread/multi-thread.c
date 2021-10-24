#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>
#include <pthread.h>

void* thread_func(void* p) {
    printf("From thread\n");
    sleep(3);
    pthread_exit(NULL);
}


int main(void)
{
    for (int i = 0; i < 4; i++)
    {
        pthread_t tid;
        pthread_create(&tid, NULL, thread_func, NULL);
        pthread_detach(tid);
    }

    pthread_exit(0);

    // for (int i = 0; i < 4; i++)
    // {
    //     pthread_join(tid[i], NULL);
    // }
    
    // printf("over");

    return 0;
}

