#include <netinet/in.h> 
#include <queue>
#include <iostream>
#include <stdio.h> 
#include <stdlib.h> 
#include <unistd.h> 
#include <string.h> 
#include <arpa/inet.h>
#include <thread>

#define MAXLINE 1024 

using namespace std;

class CapteurDriver
{
	public:
		queue<string> dataReceived;
        //2 ports available : 8080 and 8081
		bool start(uint16_t _port);
		void stop();

	private:
		thread threadCycle;
		int sockfd; 
		struct sockaddr_in servaddr;
		bool killCycle = false;

		void cycle();
};