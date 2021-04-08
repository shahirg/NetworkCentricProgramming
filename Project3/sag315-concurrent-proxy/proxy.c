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
#include <pthread.h>

#define SIZE 8192
#define PROXY_LOG "proxy.log"
#define MAX_THREADS 16

/*
 * Globals
 */
FILE *log_file; /* Log file with one line per HTTP request */

typedef struct thread_data
{
    int connfd;
    struct sockaddr_in clientaddr;
    int clientlen;
} tdata_t;
/*
 * Functions not provided to the students
 */
int open_clientfd(char *hostname, int port);
ssize_t Rio_readn_w(int fd, void *ptr, size_t nbytes);
ssize_t Rio_readlineb_w(rio_t *rp, void *usrbuf, size_t maxlen);
void Rio_writen_w(int fd, void *usrbuf, size_t n);

/*
 * Function prototypes
 */
int parse_uri(char *uri, char *target_addr, char *path, int *port);
void format_log_entry(char *logstring, struct sockaddr_in *sockaddr, char *uri, int size);

// Listens for keyboard interrupt
void sig_handler(int signo)
{
    //prints that the program is closing
    if (signo == SIGINT)
    {
        printf("\nProxy Server closing\n");
        exit(EXIT_SUCCESS);
    }
}

void *new_connection(int connfd, struct sockaddr_in clientaddr, int clientlen)
{
    //int clientlen;           /* Size in bytes of the client socket address */
    int request_count = 0; /* Number of requests received so far */

    int serverfd;            /* Socket descriptor for talking with end server */
    int response_len;        /* Total size in bytes of response from end server */
    int n;                   /* General index and counting variables */
    char *serverport;        /* Port number extracted from request URI (default 80) */
    char log_entry[MAXLINE]; /* Formatted log entry */

    //rio_t rio;         /* Rio buffer for calls to buffered rio_readlineb routine */
    char buf[MAXLINE]; /* General I/O buffer */

    //Used to fix a bug
    //int error = 0; /* Used to detect error in reading requests */
    char *httpString = " HTTP/1.0\r\n\r\n";
    struct flock lock;

    recv(connfd, buf, MAXBUF, 0);

    char url[5000], method[100], ext[1000];
    //url ex http://www.example.com/
    //method = GET
    //ext ex = HTTP/1.1

    //listen for a request
    sscanf(buf, "%s %s %s", method, url, ext);

    //print out the http request
    printf("\n%s %s %s\n", method, url, ext);
    printf("ext: %s\n", ext);

    if (strncmp(method, "GET", strlen("GET")))
    {
        printf("process_request: Received non-GET request\n");
        close(connfd);
    }

    char *host = strstr(url, "www");
    char *hostCopy = malloc(strlen(host) + 1);
    strcpy(hostCopy, host);
    printf("\n%s %s\n", host, hostCopy);

    // get the www.<domain name>.<end> portion
    host = index(host, ':') == NULL ? (char *)strtok(host, "/") : (char *)strtok(host, ":");

    printf("\nhost=%s\n", host);

    //check if a port is specified
    serverport = NULL;
    // cut off the first /
    serverport = (char *)strtok(hostCopy, "/");
    //cut off before the :
    serverport = (char *)strstr(hostCopy, ":");

    //no port default to port 80
    if (serverport == NULL)
    {
        if ((serverfd = open_clientfd(host, 80)) < 0)
            exit(EXIT_FAILURE);
    }
    else
    {
        ++serverport; //cut off first character ex: :80 -> 80
        if ((serverfd = open_clientfd(host, atoi(serverport))) < 0)
            exit(EXIT_FAILURE);
    }

    // send GET request to server
    Rio_writen(serverfd, "GET ", strlen("GET "));
    Rio_writen(serverfd, url, strlen(url));
    Rio_writen(serverfd, httpString, strlen(httpString));

    response_len = 0;
    // get result
    while ((n = Rio_readn(serverfd, buf, MAXLINE)) > 0)
    {
        response_len += n;
        Rio_writen(connfd, buf, n);
        // print out http response
        // printf("HTTP RESPONSE:\n%s\n", buf);
        // clear buffer
        bzero(buf, MAXLINE);
    }

    /* init the flock struct */
    memset(&lock, 0, sizeof(lock));
    //lock file
    lock.l_type = F_WRLCK;
    fcntl(fileno(log_file), F_SETLKW, &lock);

    // logging functionality
    format_log_entry(log_entry, &clientaddr, host, response_len);
    printf("%s\n", log_entry);
    // print log to file is it opens
    fprintf(log_file, "%s\n", log_entry);
    fflush(log_file);

    //unlock file
    lock.l_type = F_UNLCK;
    fcntl(fileno(log_file), F_SETLKW, &lock);

    // close
    //close(connfd);
    close(serverfd);
    return NULL;
}
/* 
 * main - Main routine for the proxy program 
 */
int main(int argc, char **argv)
{
    int listenfd;                  /* The proxy's listening descriptor */
    int port;                      /* The port the proxy is listening on */
    int clientlen;                 /* Size in bytes of the client socket address */
    int request_count = 0;         /* Number of requests received so far */
    struct sockaddr_in clientaddr; /* Clinet address structure*/
    int connfd;                    /* socket desciptor for talkign wiht client*/
    // int serverfd;                  /* Socket descriptor for talking with end server */
    // char *request;                 /* HTTP request from client */
    // char *request_uri;             /* Start of URI in first HTTP request header line */
    // char *request_uri_end;         /* End of URI in first HTTP request header line */
    // char *rest_of_request;         /* Beginning of second HTTP request header line */
    // int request_len;  /* Total size of HTTP request */
    // int response_len; /* Total size in bytes of response from end server */
    // int n;            /* General index and counting variables */
    // int realloc_factor; /* Used to increase size of request buffer if necessary */

    //char hostname[MAXLINE];  /* Hostname extracted from request URI */
    //char pathname[MAXLINE];  /* Content pathname extracted from request URI */
    // char *serverport;        /* Port number extracted from request URI (default 80) */
    // char log_entry[MAXLINE]; /* Formatted log entry */

    //rio_t rio;         /* Rio buffer for calls to buffered rio_readlineb routine */
    // char buf[MAXLINE]; /* General I/O buffer */

    //Used to fix a bug
    //int error = 0; /* Used to detect error in reading requests */
    int threading;
    // char *httpString = " HTTP/1.0\r\n\r\n";
    // char url[5000], method[100], ext[1000];
    pid_t p;
    pthread_t *tid;
    struct tdata_t *tdata;

    int t;

    /* Check arguments */
    if (argc != 3)
    {
        fprintf(stderr, "Usage Processes: %s <port number> -p\nUsage Processes: %s <port number> -p\n", argv[0], argv[0]);
        exit(0);
    }

    // add SIGINT to the sig handler
    // allows for termination of the proxy without killing terminal
    signal(SIGINT, sig_handler);

    // get port number
    port = atoi(argv[1]);
    printf("Port Num: %d\n", port);

    if (strcmp("-p", argv[2]))
    {
        threading = 1;
    }

    //int sockfd, clientfd;
    // create socket
    if ((listenfd = Open_listenfd(port)) < 0)
    {
        exit(1); //exit on error
    }
    printf("Listening on port %d\n", port);

    /* Inititialize */
    log_file = Fopen(PROXY_LOG, "a");
    if (threading)
    {
        //100 conncurrent users
        tid = malloc(sizeof(pthread_t) * 100);
        t = 0;
    }
    // indefinite loop
    for (;;)
    {

        clientlen = sizeof(clientaddr);

        //accept server connections
        if ((connfd = Accept(listenfd, (struct sockaddr *)&clientaddr, (socklen_t *)&clientlen)) < 0)
            exit(EXIT_FAILURE);

        if (threading)
        {
            tdata = (tdata_t *)malloc(sizeof(tdata_t));

            Pthread_create(tid[t], NULL, &new_connection, NULL);
            t = t == MAX_THREADS ? 0 : t + 1;
        }
        else
        {
            p = Fork();
            wait(NULL);
            if (p == 0)
            {
                new_connection(connfd, clientaddr, clientlen);
            }
            close(connfd);
        }

        // recv(connfd, buf, MAXBUF, 0);

        // //url ex http://www.example.com/
        // //method = GET
        // //ext ex = HTTP/1.1

        // //listen for a request
        // printf("buf = %s\n", buf);
        // sscanf(buf, "%s %s %s", method, url, ext);

        // //print out the http request
        // printf("\n%s %s %s\n", method, url, ext);

        // if (strncmp(method, "GET", strlen("GET")))
        // {
        //     printf("process_request: Received non-GET request\n");
        //     close(connfd);
        //     break;
        // }

        // char *host = strstr(url, "www");
        // char *hostCopy = malloc(strlen(host) + 1);
        // strcpy(hostCopy, host);
        // printf("\n%s %s\n", host, hostCopy);

        // // get the www.<domain name>.<end> portion
        // host = index(host, ':') == NULL ? (char *)strtok(host, "/") : (char *)strtok(host, ":");

        // printf("\nhost=%s\n", host);

        // //check if a port is specified
        // serverport = NULL;
        // // cut off the first /
        // serverport = (char *)strtok(hostCopy, "/");
        // //cut off before the :
        // serverport = (char *)strstr(hostCopy, ":");

        // //no port default to port 80
        // if (serverport == NULL)
        // {
        //     if ((serverfd = open_clientfd(host, 80)) < 0)
        //         exit(EXIT_FAILURE);
        // }
        // else
        // {
        //     ++serverport; //cut off first character ex: :80 -> 80
        //     if ((serverfd = open_clientfd(host, atoi(serverport))) < 0)
        //         exit(EXIT_FAILURE);
        // }

        // // send GET request to server
        // Rio_writen(serverfd, "GET ", strlen("GET "));
        // Rio_writen(serverfd, url, strlen(url));
        // Rio_writen(serverfd, httpString, strlen(httpString));

        // response_len = 0;
        // // get result
        // while ((n = Rio_readn(serverfd, buf, MAXLINE)) > 0)
        // {
        //     response_len += n;
        //     Rio_writen(connfd, buf, n);
        //     // print out http response
        //     // printf("HTTP RESPONSE:\n%s\n", buf);
        //     // clear buffer
        //     bzero(buf, MAXLINE);
        // }

        // // logging functionality
        // format_log_entry(log_entry, &clientaddr, host, response_len);
        // printf("%s\n", log_entry);
        // // print log to file is it opens
        // fprintf(log_file, "%s\n", log_entry);
        // fflush(log_file);

        // // close
        // close(connfd);
        // close(serverfd);
    }
    exit(0);
}

/*
 * Rio_readn_w - A wrapper function for rio_readn (csapp.c) that
 * prints a warning message when a read fails instead of terminating
 * the process.
 */
ssize_t Rio_readn_w(int fd, void *ptr, size_t nbytes)
{
    ssize_t n;

    if ((n = rio_readn(fd, ptr, nbytes)) < 0)
    {
        printf("Warning: rio_readn failed\n");
        return 0;
    }
    return n;
}

/*
 * Rio_readlineb_w - A wrapper for rio_readlineb (csapp.c) that
 * prints a warning when a read fails instead of terminating 
 * the process.
 */
ssize_t Rio_readlineb_w(rio_t *rp, void *usrbuf, size_t maxlen)
{
    ssize_t rc;

    if ((rc = rio_readlineb(rp, usrbuf, maxlen)) < 0)
    {
        printf("Warning: rio_readlineb failed\n");
        return 0;
    }
    return rc;
}

/*
 * Rio_writen_w - A wrapper function for rio_writen (csapp.c) that
 * prints a warning when a write fails, instead of terminating the
 * process.
 */
void Rio_writen_w(int fd, void *usrbuf, size_t n)
{
    if (rio_writen(fd, usrbuf, n) != n)
    {
        printf("Warning: rio_writen failed.\n");
    }
}

/*
 * parse_uri - URI parser
 * 
 * Given a URI from an HTTP proxy GET request (i.e., a URL), extract
 * the host name, path name, and port.  The memory for hostname and
 * pathname must already be allocated and should be at least MAXLINE
 * bytes. Return -1 if there are any problems.
 */
int parse_uri(char *uri, char *hostname, char *pathname, int *port)
{
    char *hostbegin;
    char *hostend;
    char *pathbegin;
    int len;

    if (strncasecmp(uri, "http://", 7) != 0)
    {
        hostname[0] = '\0';
        return -1;
    }

    /* Extract the host name */
    hostbegin = uri + 7;
    hostend = strpbrk(hostbegin, " :/\r\n\0");
    len = hostend - hostbegin;
    strncpy(hostname, hostbegin, len);
    hostname[len] = '\0';

    /* Extract the port number */
    *port = 80; /* default */
    if (*hostend == ':')
        *port = atoi(hostend + 1);

    /* Extract the path */
    pathbegin = strchr(hostbegin, '/');
    if (pathbegin == NULL)
    {
        pathname[0] = '\0';
    }
    else
    {
        pathbegin++;
        strcpy(pathname, pathbegin);
    }

    return 0;
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
