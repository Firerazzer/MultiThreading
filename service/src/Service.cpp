#include "Service.hpp"
#include <chrono>
#define n 5

Service::Service(uint16_t _port, int protocole) {
    this->protocole = protocole;

    //Lance la lecture du capteur
    driver.start(_port);

    // ftok to generate unique key 
    key_t keyKick = ftok("shmkick",65); 
    key_t keyState = ftok("shmstate",66); 

    // shmget returns an identifier in shmid 
    shmidKick = shmget(keyKick,sizeof(bool),0666|IPC_CREAT);
    shmidState = shmget(keyState,sizeof(bool),0666|IPC_CREAT);

    //lance le cycle du service
    killCycle = false;
    this->threadCycle = thread(&Service::cycle, this);
}

Service::~Service() {
    killCycle = true;
    if(threadCycle.joinable())
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

void Service::loadMemory(){
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

bool Service::readStateWatchdog() {
    auto p_state = (bool*) shmat(shmidState,(void*)0,0);
    auto state = *p_state; 
    shmdt(p_state); 
    return state;
}

void Service::cycle() {
    while (!killCycle)
    {
        // Processing in PRIMARY mode
        if(this->protocole == Protocole::PRIMARY) {
            kickWatchdog();
            //cout << this->driver.dataReceived.size() << endl;
            if(this->driver.dataReceived.size() > 0) {
              //  cout << "coucou" << endl;
                this->storage.push_back(atoi(this->driver.dataReceived.front().c_str()));
                this->driver.dataReceived.pop();
                cout << "output : " << calculOutput() << endl;
                saveMemory(this->storage.back());
            }

        // Processing in BACKUP mode
        } else {
            auto primaryAlive = readStateWatchdog();
            if(!primaryAlive) {
                loadMemory();
                this->protocole = Protocole::PRIMARY;
            }
        }
        usleep(50000);
    }
}

void Service::kickWatchdog() {
    this->p_kick = (bool*) shmat(shmidKick,(void*)0,0);
    *p_kick = true; 
    shmdt(p_kick); 
}

int main(int argc, char const *argv[])
{
    //auto start = std::chrono::high_resolution_clock::now();
	Service srv(8080, Protocole::PRIMARY);
    //srv.loadMemory();
    //auto finish = std::chrono::high_resolution_clock::now();
    //std::chrono::duration<double> elapsed = finish - start;
    //std::cout << "Elapsed time: " << elapsed.count() << " s\n";

	//sleep(100);
    //srv.display(srv.calculOutput());
    return 0;
}