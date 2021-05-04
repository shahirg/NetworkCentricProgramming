#include <pthread.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>

void *function_t_one()
{
    printf("This is the first thread created.\n");
    return (NULL);
}
void *function_t_two()
{
    printf("This is the second thread created.\n");
    return (NULL);
}

int main(int argc, char **argv)
{
    printf("This is the main thread.\n");
    pthread_t tid1, tid2;
    if ((pthread_create(&tid1, NULL, &function_t_one, NULL)) != 0)
    {
        perror("Thread 1 creation failed\n");
        exit(1);
    }
    if ((pthread_create(&tid1, NULL, &function_t_two, NULL)) != 0)
    {
        perror("Thread 2 creation failed\n");
        exit(1);
    }
    pthread_join(tid1, NULL);
    pthread_join(tid2, NULL);

    return 0;
}