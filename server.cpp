

// http://www.linuxhowtos.org/C_C++/socket.htm

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>

#include <string>
#include <string.h>

#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/wait.h>
#include <signal.h>
#include <time.h>

#include "header.h"
#include "endianness.h"
 
#define MYPORT 3490
#define BACKLOG 10      // how many pending connections queue will hold

void sigchld_handler(int s)
{
    while(wait(NULL) > 0);
}

int main(void)
{
    int sockfd, new_fd;             // listen on sock_fd, new connection on new_fd
    struct sockaddr_in my_addr;     // Sender's address information
    struct sockaddr_in their_addr;  // connector’s address information
    socklen_t sin_size;
    struct sigaction sa;
    int yes = 1;
    int n = 0;

    struct RTUDATA rtu;
    rtu.x = hton8(17);
    rtu.y = hton32(2924);

    if ((sockfd = socket(AF_INET, SOCK_STREAM, 0)) == -1) {
        perror("socket");
        exit(1);
    }
    
    if (setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof(int)) == -1) {
        perror("setsockopt");
        exit(1);
    }
    
    my_addr.sin_family = AF_INET;           // host byte order
    my_addr.sin_port = htons(MYPORT);       // short, network byte order
    my_addr.sin_addr.s_addr = INADDR_ANY;   // automatically fill with my IP
    memset(&(my_addr.sin_zero),'\0', 8);    // zero the rest of the struct

    if (bind(sockfd, (struct sockaddr *)&my_addr, sizeof(struct sockaddr))== -1) {
        perror("bind");
        exit(1);
    }
 
    if (listen(sockfd, BACKLOG) == -1) {
        perror("listen");
        exit(1);
    }
 
    sa.sa_handler = sigchld_handler;    // clear all dead processes
    sigemptyset(&sa.sa_mask);
    sa.sa_flags = SA_RESTART;
    if (sigaction(SIGCHLD, &sa, NULL) == -1) {
        perror("sigaction");
        exit(1);
    }

    while(1) {
        // main accept() loop
        sin_size = sizeof(struct sockaddr_in);
 
        if ((new_fd = accept(sockfd, (struct sockaddr *)&their_addr, &sin_size)) == -1) {
            perror("accept");
            continue;
        }
 
        printf("server: got connection from %s\n", inet_ntoa(their_addr.sin_addr));

        printf("Data to be transferred is: %d\n", (size_t)sizeof(rtu));
 
        if (!fork()) {          // this is the child process
            close(sockfd);      // child doesn’t need the listener
            /*
            if (send(new_fd, str, 15, 0) == -1)
                perror("send");
            */
            n = sendto(new_fd, &rtu, sizeof(struct RTUDATA), 0, (struct sockaddr *)&their_addr, sizeof(their_addr));
            //n = send(new_fd, (void *)&rtu, sizeof(rtu), 0);

            if ( n== -1)
                perror("send");
            else 
                printf("Total Data sent to %s is: %d \n",inet_ntoa(their_addr.sin_addr), n);
            
            close(new_fd);
            exit(0);
        }       // child process if condition close 
        close(new_fd);      // parent doesn’t need this
    }       // while loop close
 
    return 0;
}
