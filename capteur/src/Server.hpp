#include <netinet/in.h> 

class Server
{
    int sockfd; 
	struct sockaddr_in cliaddr;

	public:
		bool start();
		void stop();
		void sendToAll(const char* data, unsigned int len);
};
