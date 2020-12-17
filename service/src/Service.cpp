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

list<int> Service::loadMemory(int n){
    ifstream memory ("service/data/memory");
    string line;

    list<int> valueLoaded;
    list<int>::iterator it = valueLoaded.begin();

    int i =0;

    if(memory.is_open()){
        while ( getline(memory,line) && i != n)
        {
            i++;
            int value = stoi(line);
            valueLoaded.push_back(value);
            it++;
            cout << *it << '\n';

        }
        memory.close();
    }
    else
    {
        cout << "Le fichier n'a pas pu être ouvert!" << '\n';
        storage.push_back(-1);
    }

    return valueLoaded;
}

int Service::calculOutput(list<int> list) {
    int i = 0;
    for(int nb : list) {
        if(i == list.size()/2) {
            return nb;
        }
        i++;
    }
}

void Service::display(int result) {
    cout << "Result : " << result << "\n";
}

void Service::cycle() {
    while (!killCycle)
    {
        if(this->driver.dataReceived.size() > 0) {
            display(atoi(this->driver.dataReceived.front().c_str()));
            this->driver.dataReceived.pop();
        }
        usleep(50000);
    }
    
}

int main(int argc, char const *argv[])
{
	Service srv(8080);
    int n = 5;
    srv.saveMemory(n);
    list<int> l = srv.loadMemory(n);
    int r = srv.calculOutput(l);
    srv.display(r);
	sleep(10);
    return 0;
}