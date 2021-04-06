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
    int socketfd, clientread, addrlen = sizeof(address);
    char buffer[SIZE] = {0};

    memset(&address, '0', addrlen);
    address.sin_family = AF_INET;
    address.sin_port = htons(PORT);

    if ((socketfd = socket(AF_INET, SOCK_STREAM, 0)) < 0)
    {
        perror("Socket Creation Failed\n");
        exit(1);
    }
    if (inet_pton(AF_INET, "127.0.0.1", &address.sin_addr) <= 0)
    {
        printf("Invalid address\n");
        exit(1);
    }
    if (connect(socketfd, (struct sockaddr *)&address, addrlen) < 0)
    {
        printf("Connection Failed\n");
        exit(1);
    }

    clientread = read(socketfd, buffer, SIZE);
    printf("%s\n", buffer);
    return 0;
}