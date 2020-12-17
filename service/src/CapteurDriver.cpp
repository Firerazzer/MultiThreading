#include "CapteurDriver.hpp"

bool CapteurDriver::start(uint16_t _port) {
  
    // Creating socket file descriptor 
    if ( (this->sockfd = socket(AF_INET, SOCK_DGRAM, 0)) < 0 ) { 
        perror("socket creation failed"); 
        exit(EXIT_FAILURE); 
    } 
  
    memset(&servaddr, 0, sizeof(servaddr)); 
      
    // Filling server information 
    servaddr.sin_family = AF_INET; 
    servaddr.sin_port = htons(_port); 
    // servaddr.sin_addr.s_addr = INADDR_ANY; 
    inet_aton("127.0.0.255", &servaddr.sin_addr);

    // Bind the socket with the server address 
    if ( bind(this->sockfd, (const struct sockaddr *)&servaddr,  
            sizeof(servaddr)) < 0 ) 
    { 
        perror("bind failed"); 
        exit(EXIT_FAILURE); 
    }

    this->threadCycle = thread(&CapteurDriver::cycle, this);
  
    return true; 
}


void CapteurDriver::cycle() {
    while (!killCycle)
    {
        char buffer[MAXLINE];
      
        int n;
        socklen_t len;
            
        n = recvfrom(sockfd, (char *)buffer, MAXLINE,  
                    MSG_WAITALL, (struct sockaddr *) &servaddr, 
                    &len); 
        buffer[n] = '\0';
        this->dataReceived.push(buffer);
        usleep(50000);
    }
    
}

void CapteurDriver::stop() {
    this->killCycle = true;
    this->threadCycle.join();
    close(this->sockfd);
}