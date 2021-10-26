#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <time.h>
#include <sys/shm.h>
#include <sys/mman.h>
#include <sys/ipc.h>

#define BUF_SZ 10
#define MAX_MICRO_SECONDS 9990000

typedef struct {
	int in;
	int out;
	int buffer[BUF_SZ];
} block;

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

    // create the shared buffer
    void* ptr = mmap(NULL, sizeof(block), PROT_READ | PROT_WRITE, MAP_ANONYMOUS | MAP_SHARED, -1, 0);

    pid_t pid;
    if((pid = fork()) == 0) {
        close(fd[0]);

        int c = 0, interval;
		block* shared_block = ptr;
		
		// set the seed
		srand((unsigned) time(NULL));
		
		while(c < n){
			// wait till buffer is not full
			while((shared_block->in + 1) % BUF_SZ == shared_block->out);
			
			shared_block->buffer[shared_block->in] = d * c;
			shared_block->in = (shared_block->in + 1) % BUF_SZ;

			// wait for a random interval of time (0 to 9.999 seconds)
			interval = rand() % 10000;
			usleep(interval * 1000);
            write(fd[1], &shared_block, sizeof(block));
			
			c++;
		}

        // int numbers[n];

        // for(int i=0; i < n; i++) {
        //     int value = i * d;
        //     numbers[i] = value;
        //     usleep(get_random_number());
        // }

        // write(fd[1], &numbers, sizeof(int[n]));
        close(fd[1]);

        exit(0);
    } else {
        close(fd[1]);

        int c = 0;
		block* shared_block;
		
		while(c < n){
			// wait till buffer is not empty
			while(shared_block->in == shared_block->out);
			
			c++;
            read(fd[0], &shared_block, sizeof(block));
			printf("%d ", shared_block->buffer[shared_block->out]);
			fflush(stdout);

			shared_block->out = (shared_block->out+ 1) % BUF_SZ;
		}

		printf("\n");
		fflush(stdout);

        // int y[n];
        // read(fd[0], &y, sizeof(int[n]));

        // for (int i = 0; i < n; i++)
        // {        
        //     printf("%d\n", y[i]);
        // }
        close(fd[0]);
    }
    munmap(ptr,sizeof(block));
    return 0;
}


float get_random_number() {
    srand(time(NULL));
    float random_value = random() / 1000.00;
    return (random_value > MAX_MICRO_SECONDS) ? 
        get_random_number() : random_value;
}