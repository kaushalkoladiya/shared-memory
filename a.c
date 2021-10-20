#include <stdio.h>
// for exit(1)
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>

int main(int argc, char const *argv[])
{    
    if(!argv[1]) {
        printf("Hi tehre");
    }

    // printf("%s",argv);
    return 0;
}



// int main() {
//     pid_t pid = fork(); 
//     int n, d, in, out;
//     if(pid < 0) exit(1);
    
//     if(pid == 0) {
//         printf("Child process started\n");
//     } else {
//         wait(NULL);
//         printf("Parent process done\n");
//     }
//     return 0;
// }