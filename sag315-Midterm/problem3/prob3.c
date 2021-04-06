#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>

int main(int argc, char **argv)
{
    pid_t p;
    if ((p = fork()) < 0)
    {
        perror("Process creation Failed\n");
        exit(1);
    }
    else if (p == 0)
    {
        printf("I'm the child process\n");
    }
    else
    {
        printf("I'm the parent process\n");
    }
    return 0;
}