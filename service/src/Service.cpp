#include "Service.hpp"
#include <chrono>

Service::Service(uint16_t _port) {
    //Lance la lecture du capteur
    driver.start(_port);

    // ftok to generate unique key 
    key_t key = ftok("shmkick",65); 
    // shmget returns an identifier in shmid 
    shmid = shmget(key,sizeof(bool),0666|IPC_CREAT);

    //lance le cycle du service
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
        kickWatchdog();
        if(this->driver.dataReceived.size() > 0) {
            this->storage.push_back(atoi(this->driver.dataReceived.front().c_str()));
            this->driver.dataReceived.pop();
            cout << "output : " << calculOutput() << endl;
            saveMemory(this->storage.back());
        }
        usleep(50000);
    }
    
}

void Service::kickWatchdog() {
    this->p_kick = (bool*) shmat(shmid,(void*)0,0);
    *p_kick = true; 
    shmdt(p_kick); 
}

int main(int argc, char const *argv[])
{
    auto start = std::chrono::high_resolution_clock::now();
	Service srv(8080);
    int n = 5;
    srv.loadMemory(n);
    auto finish = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> elapsed = finish - start;
    std::cout << "Elapsed time: " << elapsed.count() << " s\n";

	sleep(100);
    srv.display(srv.calculOutput());
    return 0;
}