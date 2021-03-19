#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>

int sum(int start, int end, int total) {
    return start == end ? total + start : sum(start + 1, end, total + start);
}
int main(int argc, char **argv) {
    
    int fd[2], start, end, total = 0;
    pid_t p;

    // check input args
    if(!(argc == 1) && !(argc ==3)){
        perror("usage: ./sum\nusage: ./sum <start> <end>\n");
        exit(1);
    }

    // set summation bounds
    start = (argc == 3) ? atoi(argv[1]): 1;
    end = (argc == 3) ? atoi(argv[2]): 100;

    // pipe fail
    if (pipe(fd) < 0){
        perror("Failed to Pipe");
        exit(1);
    }

    // fork fail
    if ((p = fork()) < 0){
        perror("Process creation failed");
        exit(1);
    }
    else if (p == 0){
        // child process
        close(fd[0]);
        printf("Hi im the child\n");
        total += sum(start, end/2, total);
        printf("child sum %d - %d = %d\n", start, end/2,total);
        write(fd[1], &total, sizeof(int));
        
    }
    else {
        // parent
        wait(NULL);
        printf("Hi im the parent\n");
        close(fd[1]);
        read(fd[0], &total, sizeof(int));
        total = sum(end/2+1, end, total);
        printf("Summation %d - %d = %d\n", start,end,total);
    }
    
    return 0;
}