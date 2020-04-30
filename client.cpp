

// http://www.linuxhowtos.org/C_C++/socket.htm

#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <netdb.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <sys/socket.h>

#include "header.h"
#include "endianness.h"

#define PORT 3490           // the port client will be connecting to
#define MAXDATASIZE 1024    // max number of bytes we can get at once

int main(int argc, char *argv[])
{
    int sockfd;
    //int numbytes;
    //char buf[MAXDATASIZE];
 
    struct hostent *he;
    struct sockaddr_in their_addr; // connector’s (Sender's) address information
    if (argc != 2) {
        fprintf(stderr,"usage: client hostname\n");
        exit(1);
    }
    
    if ((he=gethostbyname(argv[1])) == NULL) {
        perror("gethostbyname");
        exit(1);
    }
    // get the host info
    if ((sockfd = socket(AF_INET, SOCK_STREAM, 0)) == -1) {
        perror("socket");
        exit(1);
    }
    
    their_addr.sin_family = AF_INET;    // host byte order
    their_addr.sin_port = htons(PORT);    // short, network byte order
    their_addr.sin_addr = *((struct in_addr *)he->h_addr);
    memset(&(their_addr.sin_zero),'\0', 8);    // zero the rest of the struct
    
    if (connect(sockfd, (struct sockaddr *)&their_addr, sizeof(struct sockaddr)) == -1) {
        perror("connect");
        exit(1);
    }
    /*
    if ((numbytes=recv(sockfd, buf, MAXDATASIZE-1, 0)) == -1) {
    perror("recv");
    exit(1);
    }
    */
    

    struct RTUDATA rtu;
    
    //RECEIVING DATA
    //n = recvfrom(sock, &pkt, sizeof(struct packet), 0, (struct sockaddr *)&from, &fromlen);

    
    socklen_t fromLen = sizeof(their_addr);

    if (recvfrom(sockfd, (struct RTUDATA *)&rtu, sizeof(rtu), 0,  (struct sockaddr *)&their_addr, &fromLen) == -1) {
        perror("recv");
        exit(1);
    }

    // fix byte order
    uint8_t x = ntoh8(rtu.x);
    uint32_t y = ntoh32(rtu.y);
    
    std::cout << "Struct reveived size is = " << sizeof(rtu) << std::endl;

    std::cout << "x: " << unsigned(x) << std::endl;    
    std::cout << "y: " << y << std::endl;

    // printf("%d :: %c :: %d :: %s",ntohs(rtu.tagid),rtu.flag,ntohs(rtu.value),rtu.time_stamp);
    
    
    close(sockfd);
    return 0;
}





/*
#include <iostream>
#include <sys/types.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <string.h>
#include <string>


int main()
{
    // create a socket
    int sock = socket(AF_INET, SOCK_STREAM, 0);
    if (sock == -1) {
        return 1;
    }

    // Create a hint structure for the server we're connecting with
    int port = 54000;
    std::string ipAddress = "127.0.0.1";

    sockaddr_in hint;
    hint.sin_family = AF_INET;
    hint.sin_port = htons(port);
    inet_pton(AF_INET, ipAddress.c_str(), &hint.sin_addr);

    // Connect to the server on the socket
    int connectRes = connect(sock, (sockaddr*)&hint, sizeof(hint));
    if (connectRes == -1) {
        return 1;
    }

    // while loop:
    char buf[4096];
    std::string userInput;

    do {
        std::cout << "> ";
        getline(std::cin, userInput);

        int sendRes = send(sock, userInput.c_str(), userInput.size() + 1, 0);
        if (sendRes == -1) {
            std::cout << "Could not send to server! Whoops!\r\n";
            continue;
        }

        // Wait for response
        memset(buf, 0, 4096);
        int bytesReceived = recv(sock, buf, 4096, 0);
        if (bytesReceived == -1) {
            std::cout << "There was an error getting response from server\r\n";
        } else {
            // Display response
            std::cout << "SERVER> " << std::string(buf, bytesReceived) << "\r\n";
        }
    } while (true);
    
    close(sock);

    return 0;
}
*/