#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <string.h>
#include <stdlib.h>

int main()
{
    int pipefd[2];
    pipe(pipefd);
    int sum = 0;
    pid_t p;
    if ((p = fork()) < 0)
    {
        perror("Process Creation Failed");
        exit(1);
    }
    else if (p == 0)
    {
        close(pipefd[0]);
        for (int i = 1; i <= 100; i++)
        {
            if (i % 2 == 0)
            {
                sum = sum + i;
            }
        }
        write(pipefd[1], &sum, sizeof(int));
    }
    else
    {
        wait(NULL);
        close(pipefd[1]);
        read(pipefd[0], &sum, sizeof(int));
        for (int i = 1; i <= 100; i++)
        {
            if (i % 2 != 0)
            {
                sum = sum + i;
            }
        }
        printf("Summation = %d\n", sum);
    }
    return(0);
}