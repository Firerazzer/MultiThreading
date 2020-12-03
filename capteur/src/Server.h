#include <memory>

using namespace std;

class Server
{
	public:
		Server();
		Server(const Server&) = delete;
		Server& operator=(const Server&) = delete;
		Server(Server&&);
		Server& operator=(Server&&);
		~Server();

		bool start(unsigned short _port);
		void stop();
		void update();
		unique_ptr<Messages::Base> poll();

	private:
		unique_ptr<Server> mImpl;
};