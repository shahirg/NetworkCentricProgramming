#include <stdio.h>
#include <sys/socket.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#define PORT 8080
#define SIZE 1024

int main(int argc, char **argv)
{
    struct sockaddr_in address;
    int serverfd, listenfd, addrlen = sizeof(address);
    char buffer[SIZE] = {0};
    char *message = "Network-Centric Programming – Spring 2021 Midterm";

    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(PORT);

    if ((serverfd = socket(AF_INET, SOCK_STREAM, 0)) < 0)
    {
        perror("Socket Creation Failed");
        exit(1);
    }
    if (bind(serverfd, (struct sockaddr *)&address, addrlen) < 0)
    {
        perror("Bind Fail\n");
        exit(1);
    }
    if (listen(serverfd, SIZE) < 0)
    {
        perror("Listening Failed\n");
        exit(1);
    }

    //indefinite loop to keep the server running
    for (;;)
    {
        if ((listenfd = accept(serverfd, (struct sockaddr *)&address, (socklen_t *)&addrlen)) < 0)
        {
            perror("Could not accept connection\n");
            exit(1);
        }
        write(listenfd, message, strlen(message));
        printf("Message sent to client\n");
    }

    return 0;
}