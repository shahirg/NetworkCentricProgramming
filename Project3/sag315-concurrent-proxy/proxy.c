/*
 * proxy.c - A Simple Sequential Web proxy
 *
 * Course Name: 14:332:456-Network Centric Programming
 * Assignment 2
 * Student Name:Shahir Ghani
 * 
 * IMPORTANT: Give a high level description of your code here. You
 * must also provide a header comment at the beginning of each
 * function that describes what that function does.
 */ 

#include "csapp.h"
#include <signal.h>
#include <unistd.h>
#include <stdio.h>
#include <netinet/in.h>

#define SIZE 8192
/*
 * Function prototypes
 */
void format_log_entry(char *logstring, struct sockaddr_in *sockaddr, char *uri, int size);


// Listens for keyboard interrupt
void sig_handler(int signo){
    //prints that the program is closing
    if(signo ==SIGINT){
        printf("\nProxy Server closing\n");
        exit(EXIT_SUCCESS);
    }
    
}
/* 
 * main - Main routine for the proxy program 
 */
int main(int argc, char **argv)
{

    /* Check arguments */
    if (argc != 2) {
	fprintf(stderr, "Usage: %s <port number>\n", argv[0]);
	exit(0);
    }

    // add SIGINT to the sig handler
    // allows for termination of the proxy without killing terminal
    signal(SIGINT, sig_handler);

    // get port number
    int port = atoi(argv[1]);
    printf("Port Num: %d\n",port);

    int sockfd, clientfd; 
    // create socket
    if((sockfd = Open_listenfd(port)) < 0){
        exit(1);    //exit on error
    }
    printf("Listening on port %d\n", port);

    // indefinite loop
    for (;;)
    {
        //set up clientaddress
        struct sockaddr_in clientaddress;
        clientaddress.sin_family = AF_INET;
        clientaddress.sin_addr.s_addr = htonl (INADDR_ANY);
        clientaddress.sin_port = htons(port);
        int clientlen = sizeof(clientaddress);
        
        //accept server connections
        if ((clientfd = Accept(sockfd, (struct sockaddr *)&clientaddress, (socklen_t*)&clientlen)) < 0)
            exit(EXIT_FAILURE);

        char buffer[MAXLINE];
        recv(clientfd, buffer, MAXBUF, 0);

        char url[5000],method[100],ext[1000];
        //url ex http://www.example.com/
        //method = GET
        //ext ex = HTTP/1.1
  

        //listen for a request
        sscanf(buffer, "%s %s %s", method, url, ext);

        //print out the http request
        printf("\n%s %s%s\n",method, url, ext);

        char *host = strstr(url, "www");
        char *hostCopy = malloc(strlen(host) + 1);
        strcpy(hostCopy, host);
        printf("\n%s %s\n",host, hostCopy);

        // get the www.<domain name>.<end> portion
        host = index(host, ':')== NULL ?(char *)strtok(host, "/") : (char *)strtok(host, ":");

        printf("\nhost=%s\n",host);

        //check if a port is specified
        char *strHTTPport = NULL;
        // cut off the first /
        strHTTPport = (char *)strtok(hostCopy, "/");
        //cut off before the :
        strHTTPport = (char *)strstr(hostCopy, ":");

        int serverfd;
        //no port default to port 80
        if (strHTTPport == NULL){
            if ((serverfd = open_clientfd(host, 80)) < 0)
                exit(EXIT_FAILURE);
        }
        else {
            ++strHTTPport;//cut off first character ex: :80 -> 80
            if ((serverfd = open_clientfd(host, atoi(strHTTPport))) < 0)
                exit(EXIT_FAILURE);
        }

        // send GET request to server
        Rio_writen(serverfd, "GET ", strlen("GET "));
        Rio_writen(serverfd, url, strlen(url));
        char *httpString = " HTTP/1.0\r\n\r\n";
        Rio_writen(serverfd, httpString, strlen(httpString));

        int n,responseLen = 0;
        // get result
        while ((n = rio_readn(serverfd, buffer, MAXLINE)) > 0){
            responseLen += n;
            Rio_writen(clientfd, buffer, n);
            // print out http response
            printf("HTTP RESPONSE:\n%s\n",buffer);
            // clear buffer
            bzero(buffer, MAXLINE);
            
        }

        // logging functionality
        char logstring[SIZE];
        format_log_entry(logstring, &clientaddress, host, responseLen);
        printf("%s\n", logstring);
        
        FILE *fp;
        if ((fp = fopen("proxy.log", "a")) != NULL){
            // print log to file is it opens
            fprintf(fp, "%s\n", logstring);
            fclose(fp);
        }
        else{
            printf("FILE NOT FOUND\n");
        }
       
        // close
        close(clientfd);
        close(serverfd);
    } 
    exit(0); 
}

/*
 * format_log_entry - Create a formatted log entry in logstring. 
 * 
 * The inputs are the socket address of the requesting client
 * (sockaddr), the URI from the request (uri), and the size in bytes
 * of the response from the server (size).
 */
void format_log_entry(char *logstring, struct sockaddr_in *sockaddr, 
		      char *uri, int size)
{
    time_t now;
    char time_str[MAXLINE];
    unsigned long host;
    unsigned char a, b, c, d;

    /* Get a formatted time string */
    now = time(NULL);
    strftime(time_str, MAXLINE, "%a %d %b %Y %H:%M:%S %Z", localtime(&now));

    /* 
     * Convert the IP address in network byte order to dotted decimal
     * form. Note that we could have used inet_ntoa, but chose not to
     * because inet_ntoa is a Class 3 thread unsafe function that
     * returns a pointer to a static variable (Ch 13, CS:APP).
     */
    host = ntohl(sockaddr->sin_addr.s_addr);
    a = host >> 24;
    b = (host >> 16) & 0xff;
    c = (host >> 8) & 0xff;
    d = host & 0xff;


    /* Return the formatted log entry string */
    sprintf(logstring, "%s: %d.%d.%d.%d %s", time_str, a, b, c, d, uri);
}


