#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>
#include <math.h>
#include <pthread.h>
#include <semaphore.h>

#define MAX_NUMBER 1
#define MIN_NUMBER -1
#define TOTAL_POINTS 100000

float get_random_number();

int counter = 0;
sem_t semaphore;

void* thread_func(void* p) {
    // do circle area process here

    // sem_wait(&semaphore);
    for (int i = 0; i < TOTAL_POINTS; i++) {
        float x = get_random_number();
        float y = get_random_number();
        float r = sqrt((x*x) + (y*y));
        if(r <= 1) {
            counter++;
            sem_post(&semaphore);
        }
    }

    // pthread_exit(NULL);
}

int main(void)
{
    // reference of the variable, 0 = if want to use only on threads, initial value
    sem_init(&semaphore, 0, 0);

    pthread_t tid[4];

    for (int i = 0; i < 4; i++)
    {
        pthread_create(&tid[i], NULL, thread_func, NULL);
    }
    
    for (int i = 0; i < 4; i++)
    {
        pthread_join(tid[i], NULL);
    }

    
    int inside_points;
    sem_getvalue(&semaphore, &inside_points);

    double area = (inside_points / (TOTAL_POINTS * 4)) * 4;
    printf("Total inside points: %d\n", inside_points);
    printf("Area: %lf\n", area);

    // wait till other threads get executed.
    pthread_exit(0);    
    
    
    sem_destroy(&semaphore);
    return 0;
}


float get_random_number() {
    srand(time(NULL));
    return MIN_NUMBER + random() % (MAX_NUMBER - MIN_NUMBER +1);
}