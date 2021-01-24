#include "Server.hpp"
  
using namespace std;

// Driver code 
void Server::sendToAll(const char* data, unsigned int len) { 
    cout << data << endl;
    sendto(sockfd, data, len, MSG_CONFIRM, (struct sockaddr*) &cliaddr_1, sizeof cliaddr_1);
    sendto(sockfd, data, len, MSG_CONFIRM, (struct sockaddr*) &cliaddr_2, sizeof cliaddr_2);
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

    memset(&cliaddr_1, 0, sizeof cliaddr_1);
    cliaddr_1.sin_family = AF_INET;
    cliaddr_1.sin_port = htons(PORT);
    inet_aton("127.0.0.255", &cliaddr_1.sin_addr);

    memset(&cliaddr_2, 0, sizeof cliaddr_2);
    cliaddr_2.sin_family = AF_INET;
    cliaddr_2.sin_port = htons(PORT+1);
    inet_aton("127.0.0.255", &cliaddr_2.sin_addr);

    this->threadCycle = thread(&Server::cycle, this);

    return true;
}

void Server::cycle() {
    while (!killCycle)
    {
        if(this->toSend.size() > 0) {
            sendToAll(this->toSend.front().c_str(), this->toSend.front().length());
            this->toSend.pop();
        }
        usleep(50000);
    }
    
}

void Server::stop() {
    this->killCycle = true;
    threadCycle.join();
    close(this->sockfd);
}