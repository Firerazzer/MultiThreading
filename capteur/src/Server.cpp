#include "Server.hpp"

#include <iostream>
#include <stdio.h> 
#include <stdlib.h> 
#include <unistd.h> 
#include <string.h> 
#include <arpa/inet.h>
  
using namespace std;

#define PORT     8080 
#define MAXLINE 1024 

// Driver code 
void Server::sendToAll(const char* data, unsigned int len) { 
    cout << data << endl;
    sendto(sockfd, data, len, MSG_CONFIRM, (struct sockaddr*) &cliaddr, sizeof cliaddr);
} 


bool Server::start() {
    struct sockaddr_in servaddr; 
      
    // Creating socket file descriptor 
    if ( (this->sockfd = socket(AF_INET, SOCK_DGRAM, 0)) < 0 ) { 
        perror("socket creation failed"); 
        exit(EXIT_FAILURE); 
    } 
      
    memset(&servaddr, 0, sizeof(servaddr));
      
    // Filling server information 
    servaddr.sin_family    = AF_INET; // IPv4 
    servaddr.sin_addr.s_addr = INADDR_ANY; 
    servaddr.sin_port = htons(PORT); 
      
    // Bind the socket with the server address 
    if ( bind(this->sockfd, (const struct sockaddr *)&servaddr,  
            sizeof(servaddr)) < 0 ) 
    { 
        perror("bind failed"); 
        exit(EXIT_FAILURE); 
    } 

    memset(&cliaddr, 0, sizeof cliaddr);
    cliaddr.sin_family = AF_INET;
    cliaddr.sin_port = htons(PORT);
    // broadcasting address for unix (?)
    inet_aton("127.0.0.255", &cliaddr.sin_addr);
    //cliaddr.sin_addr.s_addr = INADDR_ANY;

    return true;
}

void Server::stop() {
    close(this->sockfd);
}