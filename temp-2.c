#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <time.h>

#define MAX_MICRO_SECONDS 9990000
#define clear() printf("\033[H\033[J")

struct node
{
	int info;
	struct node *next;
}*start=NULL,*prev,*temp,*curr;

void insert(int value);
void delete();
float get_random_number();

int main(int argc, char const *argv[]) {
    pid_t pid = fork();
    if(argc != 3) {
        printf("Missing inputs.\ni.g. <file> <value of n> <value of d>\n");
        exit(1);
    }
    
    if(pid == 0) {
        int n, d;
        sscanf(argv[1], "%d", &n);
        sscanf(argv[2], "%d", &d);
        clear();
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
    temp=(struct node *)malloc(sizeof(struct  node));
	
    if(temp==NULL)
	{
		printf("Not enought Memory\n");
		return;
	}
	
    temp->info=value;
	
    if(start==NULL)
	{
		start=temp;
		start->next=NULL;
	}
    else
	{
		temp->next=start;
		start=temp;
	}
    delete();
    usleep(get_random_number());
}

void delete() {
	curr = start;
	if(start==NULL)
	{
		printf("Buffer is empty\n");
		return;
	}
	temp=curr;
	curr=curr->next;
	printf("%d\n", curr->info);
	start=curr;
	free(temp);
}


float get_random_number() {
    srand(time(NULL));
    float random_value = random() / 1000.00;
    return (random_value > MAX_MICRO_SECONDS) ? 
        get_random_number() : random_value;
}