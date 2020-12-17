#include "Service.hpp"

Service::Service(uint16_t _port) {
    driver.start(_port);
    killCycle = false;
    this->threadCycle = thread(&Service::cycle, this);
}

Service::~Service() {
    killCycle = true;
    threadCycle.join();
    driver.stop();
}

int Service::saveMemory (int x) {
    ofstream memory;
    string valueToSave;

    valueToSave = "\n" + std::to_string(x);

    memory.open("service/data/memory", fstream::app);
    
    if(memory.is_open()){
        memory << valueToSave;
        memory.close();
        return 1;
    }
    else
    {
        cout << "Le fichier n'a pas pu être ouvert!" << '\n';
        return -1;
    }
    


    return 0;
}

void Service::loadMemory(int n){
    ifstream memory ("service/data/memory");
    string line;

    this->storage.clear();

    int i = 0;

    if(memory.is_open()){
        while ( getline(memory,line) && i != n)
        {
            i++;
            int value = stoi(line);
            this->storage.push_back(value);

        }
        memory.close();
    }
    else
    {
        cout << "Le fichier n'a pas pu être ouvert!" << '\n';
    }
}

double Service::calculOutput() {
    double sum = 0;
    for(int nb : this->storage) {
        sum += nb;
    }
    return sum/this->storage.size();
}

void Service::display(int result) {
    cout << "Result : " << result << "\n";
}

void Service::cycle() {
    while (!killCycle)
    {
        if(this->driver.dataReceived.size() > 0) {
            this->storage.push_back(atoi(this->driver.dataReceived.front().c_str()));
            this->driver.dataReceived.pop();
            cout << "output : " << calculOutput() << endl;
            saveMemory(this->storage.back());
        }
        usleep(50000);
    }
    
}

int main(int argc, char const *argv[])
{
	Service srv(8080);
    int n = 5;
    srv.loadMemory(n);
	sleep(100);
    srv.display(srv.calculOutput());
    return 0;
}