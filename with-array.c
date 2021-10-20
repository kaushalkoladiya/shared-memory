#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <time.h>

#define MAX_MICRO_SECONDS 9990000
#define BUFFER_SIZE 10

int in = -1, out = -1, buffer[BUFFER_SIZE];

void insert(int value);
void delete();
float get_random_number();

int main(int argc, char const *argv[]) {
    pid_t pid;
    if(argc != 3) {
        printf("Missing inputs.\ni.g. <file> <value of n> <value of d>\n");
        exit(1);
    }
    
    if(pid = fork() == 0) {
        int n, d;
        sscanf(argv[1], "%d", &n);
        sscanf(argv[2], "%d", &d);
        for(int i=0; i < n; i++) {
            int value = i * d;
            insert(value);
        }
        exit(-1);
    }
     else {
        printf("Parent process done\n");
    }
    return 0;
}


void insert(int value) {
    if(out==(in+1)%BUFFER_SIZE)
	{
		printf("Buffer is Overflow\n");
	}
	else
	{
		if(out==-1)
			out=0;
		in=(in+1)%BUFFER_SIZE;
		buffer[in] = value;

        delete();
        usleep(get_random_number());
	}
}

void delete()
{
	int value;
	if(out==-1)
		printf("Buffer is empty\n");
	else
	{
		value=buffer[out];
		printf("\n%d",value);
        fflush(stdout);
		if(out==in)
		{
			out=-1;
			in=-1;
		}
		else
			out=(out+1)%BUFFER_SIZE;
	}
}


float get_random_number() {
    srand(time(NULL));
    float random_value = random() / 1000.00;
    return (random_value > MAX_MICRO_SECONDS) ? 
        get_random_number() : random_value;
}