#include <sys/types.h>
#include <sys/uio.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <string.h>
#define SIZE 1024

int main(int argc, char **argv)
{

    char *filename = "file.txt";
    char *buffer;
    int flag = (argc > 1 && strcmp(argv[1], "--systemcalls") == 0) ? 1 : 0;
    if (flag)
    {
        //using system calls
        int fd;
        if ((fd = open(filename, O_RDONLY)) < 0)
        {
            perror("Opening File Failed\n");
            exit(1);
        }
        else
        {
            if ((read(fd, buffer, SIZE)) < 0)
            {
                perror("Failed to read file\n");
                exit(1);
            }
            else
            {
                printf("%s\n", buffer);
            }
        }
    }
    else
    {
        //using fileio
        FILE *pFile;
        char buf[SIZE];
        if ((pFile = fopen(filename, "r")) == NULL)
        {
            perror("Opening File Failed\n");
            exit(1);
        }
        else
        {
            while (1)
            {
                if (fgets(buf, SIZE, pFile) == NULL)
                    break;
                else
                    printf("%s\n", buf);
            }
        }
    }

    return 0;
}