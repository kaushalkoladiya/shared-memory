#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <time.h>

#define MAX_MICRO_SECONDS 9990000

float get_random_number();

int main(int argc, char const *argv[]) {
    if(argc != 3) {
        printf("Missing inputs.\ni.g. <file> <value of n> <value of d>\n");
        exit(1);
    }

    int fd[2];
    // fd[0] - read
    // fd[1] - write
    if(pipe(fd) == -1) {
        printf("An error occured while creating pipeline!");
        return (1);
    }

    int n, d;

    sscanf(argv[1], "%d", &n);
    sscanf(argv[2], "%d", &d);

    pid_t pid;
    if((pid = fork()) == 0) {
        close(fd[0]);

        int numbers[n];

        for(int i=0; i < n; i++) {
            int value = i * d;
            numbers[i] = value;
            usleep(get_random_number());
        }

        write(fd[1], &numbers, sizeof(int[n]));
        close(fd[1]);

        exit(0);
    }
     else {
        close(fd[1]);
        int y[n];
        read(fd[0], &y, sizeof(int[n]));

        for (int i = 0; i < n; i++)
        {        
            printf("%d\n", y[i]);
        }
        close(fd[0]);
    }
    return 0;
}


float get_random_number() {
    srand(time(NULL));
    float random_value = random() / 1000.00;
    return (random_value > MAX_MICRO_SECONDS) ? 
        get_random_number() : random_value;
}