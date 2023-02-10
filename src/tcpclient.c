#include <sys/socket.h> // Internet Protocol family
#include <sys/types.h> // data types
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <stdarg.h>
#include <errno.h>
#include <fcntl.h>
#include <sys/time.h>
#include <sys/ioctl.h>
#include <netdb.h>

// standard HTTP  port
#define SERVER_PORT 80

#define MAXLINE 4096 // buffer length
#define SA struct sockaddr

void err_n_die(const char fmt, ...); // handle errors

int main(int argc, char **argv){
    // local variable
    int sockfd, n;
    int sendbytes;
    struct sockaddr_in servaddr;
    char sendline[MAXLINE];
    char recvline[MAXLINE];

    // usage check
    if (argc != 2)
        err_n_die("usage: %s <server address>", argv[0]);

    // AF_INET == Address Famaly - internet
    // 0 == use TCP
    if ( (sockfd = socket(AF_INET, SOCK_STREAM, 0)) < 0)
        err_n_die("Error while creating the socket!");
    
    bzero(&servaddr, sizeof(servaddr));
    servaddr.sin_family = AF_INET;
    servaddr.sin_port   = htons(SERVER_PORT); // htons = host to network

    // translate address to binary, "1.2.3.4" => [1, 2, 3, 4]
    if ( inet_pton(AF_INET, argv[1], &servaddr.sin_addr) <= 0 )
        err_n_die("inet_pton error for %s ", argv[1]);

    if ( connect(sockfd, (SA *) &servaddr, sizeof(servaddr)) < 0 )
        err_n_die("connect faild!");

    // we're connected. Prepare the message
    sprintf(sendline, "GET / HTTP/1.1\r\n\r\n");
    sendbytes = strlen(sendline);

    // send the request
    if (write(sockfd, sendline, sendbytes) != sendbytes)
        err_n_die("write error.");

    memset(recvline, 0, MAXLINE);

    // now reads the server's response
    while ( (n = read(sockfd, recvline, MAXLINE-1)) > 0 )
    {
        printf("%s", recvline);
    }
    
    if (n < 0)
        err_n_die("read error");

    exit(0);
}

void err_n_die(const char fmt, ...){
    int      errno_save;
    va_list  ap;

    // any system or library call can set errno
    // so we need to save it now
    errno_save = errno;

    // print out the fmt+args to standard out
    va_start(ap, fmt);
    vfprintf(stdout, fmt, ap);
    fprintf(stdout, "\n");
    fflush(stdout);

    // print out error message if errno was set.
    if (errno_save != 0){
        fprintf(
            stdout, 
            "(errno = %d) : %s\n", 
            errno_save,
            strerror(errno_save)
        );
        fprintf(stdout, "\n");
        fflush(stdout);
    }

    va_end(ap);

    // this is the ..and die part. Terminate with an erro.
    exit(1);
}