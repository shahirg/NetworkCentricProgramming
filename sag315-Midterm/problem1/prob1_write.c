#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <sys/types.h>
#include <unistd.h>
#include <pthread.h>
#include <fcntl.h>
#define SIZE 1024

int main(int argc, char **argv)
{

    char *filename = "file.txt";

    int flag = (argc > 1 && strcmp(argv[1], "--systemcalls") == 0) ? 1 : 0;
    if (flag)
    {
        int fd;
        //get message to write
        char message[SIZE];
        printf("Enter message: ");
        scanf("%[^\n]s", message);
        // 0666 gives reading permissions
        if ((fd = open(filename, O_WRONLY | O_CREAT | O_APPEND, 0666)) < 0)
        {
            perror("Opening File Failed\n");
            exit(1);
        }
        else
        {
            if ((write(fd, message, strlen(message))) < 0)
            {
                perror("Failing to write to file\n");
                exit(1);
            }
            else
            {
                printf("Write Finished using systemcalls\n");
            }
            close(fd);
        }
    }
    else
    {
        FILE *pFile;
        char message[SIZE];
        printf("Enter message: ");
        //scan until new line symbol and store in message
        scanf("%[^\n]s", message);
        if ((pFile = fopen(filename, "wb")) < 0)
        {
            perror("Opening File Failed\n");
            exit(1);
        }
        else
        {

            fputs(message, pFile);
            printf("Write Finished using fileio\n");
            fclose(pFile);
        }
    }

    return 0;
}