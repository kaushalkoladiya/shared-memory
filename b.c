#include <stdio.h>
// for exit(1)
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/shm.h>
#include <sys/ipc.h>
#include <sys/types.h>
#include <string.h>
#include <errno.h>


#define BUFFER_SIZE 1024
#define SHM_KEY 0x1234

struct shmseg
{
    int cnt;
    int complete;
    char buf[BUFFER_SIZE];
};

int fill_buffer(char * bufptr, int size);

int main(int argc, char const *argv[])
{
    
    int shmid, numtimes;
    struct shmseg *shmp;
    char *bufptr;
    int spaceavailable;
    shmid = shmget(SHM_KEY, sizeof(struct shmseg), 0664|IPC_CREAT);

    if(shmid == -1) {
        perror("Shared memory");
        return 1;
    }

    // attach to the segment to get a pointer to it
    shmp = shmat(shmid, NULL, 0);
    if(shmp == (void *) - 1) {
        perror("Shared memory attach!");
        return 1;
    }

    // transfer blocks of data from buffer to shared memory 
    
    bufptr =shmp-> buf;  
    spaceavailable = BUFFER_SIZE;

    for (numtimes = 0; numtimes <5; numtimes ++) {
        shmp->cnt = fill_buffer(bufptr, spaceavailable); 
        shmp->complete = 0; 
        printf ("Writing Process: Shared Memory Write: Wrote %d bytes \n", shmp->cnt); 
        bufptr = shmp->buf;
        spaceavailable = BUFFER_SIZE; 
        sleep(1);
    } 

    printf("Writing Process: Wrote %d times\n", numtimes); 
    shmp-> complete = 1; 
    if (shmdt(shmp) == -1){ 
        perror("shmdt"); 
        return 1;
    }



    if (shmctl (shmid, IPC_RMID, 0) == -1) 
    {
        perror("shmctl");
        return 1; 
    
    }
    printf("Writing Process: Complete \n");
    
    return 0;
}

int fill_buffer (char * bufptr, int size) {
    static char ch = 'A';
    int filled_count;
 
    memset (bufptr, ch, size - 1); 
    if (ch> 122) {
        ch = 65;
    }
    if((ch >= 65) && (ch <= 122)) {
        if((ch >= 91) && (ch <= 96)) {
            ch= 96;
        }
    }

    filled_count = strlen(bufptr);
    ch++;

    return filled_count;
}


