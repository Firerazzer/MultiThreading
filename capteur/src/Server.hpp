#include <netinet/in.h> 
#include <queue>
#include <iostream>
#include <stdio.h> 
#include <stdlib.h> 
#include <unistd.h> 
#include <string.h> 
#include <arpa/inet.h>
#include <thread>

#define PORT    8080 
#define MAXLINE 1024 

using namespace std;

class Server
{
	public:
		queue<string> toSend;
		bool start();
		void stop();

	private:
		int sockfd; 
		struct sockaddr_in cliaddr_1;
		struct sockaddr_in cliaddr_2;
		bool killCycle = false;
		thread threadCycle;

		void cycle();
		void sendToAll(const char* data, unsigned int len);
};
