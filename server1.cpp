

#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <string>
#include <sstream>
#include <vector>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
//#include <thread>

std::vector<std::string> handleRequest(std::string request)
{
    std::vector<std::string> response;

    response.push_back(std::string("Pong!"));
    response.push_back(std::string("CLOSE"));

    return response;
}

void handleConnection(int newsockfd, sockaddr_in* cli_addr)
{
    char buffer[256];
    bzero(buffer, 256);

    while (true) {
        int n = read(newsockfd, buffer, 255);
        if (n == 0) {
            std::cout << inet_ntoa(cli_addr->sin_addr) << ":" << ntohs(cli_addr->sin_port) 
                << " connection closed by client" << std:: endl;
            return;
        }
        else if (n < 0)
            std::cerr << "ERROR reading from socket" << std::endl;
        
        std::stringstream stream;
        stream << buffer << std::flush;
        while (stream.good()) {
            std::string request;
            getline(stream, request);
            if (request.length() > 0) {
                std::cout << inet_ntoa(cli_addr->sin_addr) << ":" << ntohs(cli_addr->sin_port)
                    << ": " << request << std::endl;
                
                std::vector<std::string> response = handleRequest(request);

                for (int i = 0; i < response.size(); i++) {
                    std::string output = response[i];
                    if (output != "CLOSE") {
                        n = write(newsockfd, output.c_str(), output.length());
                        if (n < 0)
                            std::cerr << "ERROR writing to socket" << std::endl;
                    }
                    else {
                        close(newsockfd);
                        std::cout << inet_ntoa(cli_addr->sin_addr) << ":" << ntohs(cli_addr->sin_port)
                            << " connection terminated" << std::endl;
                        return;
                    }
                }
            }
        }
    }
}

int main(int argc, char *argv[])
{
    int sockfd;
    int portno;

    sockaddr_in serv_addr;

    if (argc < 2) {
        std::cerr << "ERROR no port provided" << std::endl;
        exit(1);
    }

    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0) {
        std::cerr << "ERROR opening socket" << std::endl;
    }

    int reusePort = 1;
    setsockopt(sockfd, SOL_SOCKET, SO_REUSEPORT, &reusePort, sizeof(reusePort));

    bzero((char*)&serv_addr, sizeof(serv_addr));
    portno = atoi(argv[1]);

    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(portno);
    serv_addr.sin_addr.s_addr = INADDR_ANY;

    if (bind(sockfd, (sockaddr *) &serv_addr, sizeof(serv_addr)) < 0)
        std::cerr << "ERROR on binding" << std::endl;
    
    unsigned int backlogSize = 5;
    listen(sockfd, backlogSize);
    std::cout << "C++ Server opened on port " << portno << std::endl;

    while (true) {
        int newsockfd;
        unsigned int clilen;
        sockaddr_in cli_addr;

        clilen = sizeof(sockaddr_in);
        newsockfd = accept(sockfd, (sockaddr *)&cli_addr, &clilen);
        if (newsockfd < 0)
            std::cerr << "ERROR on accept" << std::endl;
        
        std::cout << inet_ntoa(cli_addr.sin_addr) << ":" << ntohs(cli_addr.sin_port)
            << " connected" << std::endl;
        
        handleConnection(newsockfd, &cli_addr);
    }
    return 0;
}
