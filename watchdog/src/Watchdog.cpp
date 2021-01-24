#include "Watchdog.hpp"
#include <string.h>

Watchdog::Watchdog(){
    stateSys = false;
    startTime = clock();
    kill = false;
    
    shmidKick = shmget(12345,sizeof(bool),0666|IPC_CREAT); 
    shmidState = shmget(12346,sizeof(bool),0666|IPC_CREAT); 
    setState(false);
    setKick(false);
}

Watchdog::~Watchdog(){
    // destroy the shared memory 
    shmctl(this->shmidKick,IPC_RMID,NULL);       
    shmctl(this->shmidState,IPC_RMID,NULL); 
}

void Watchdog::stop() {
    stateSys = false;
}

void Watchdog::start(){
    startTime = clock();
    this->stateSys = true;
    this->kill = false;
    this->setState(true);
    cout << "Backup can be launch " << readState() << endl;
    this->threadCycle = thread(&Watchdog::cycle, this);
    this->watch();
}


void Watchdog::watch(){
    while(stateSys){
        if(((clock() - startTime) / (CLOCKS_PER_SEC / 1000)) > getTimeLimit()){
            stateSys = false;
            kill = true;
            setState(false);
            cerr << "No kick during " << ((clock() - startTime) / (CLOCKS_PER_SEC / 1000000.0)) << "us\n";
            this->threadCycle.join();
        }
    } 
}

int Watchdog::getTimeLimit(){
    if(timeLimit < 0) {
        std::ifstream file ("save.txt");
        if(!file){
            std::cerr << "Impossible d'ouvrir le fichier " << std::endl;
            timeLimit = DEFAULT_TIME_LIMIT;
        } else{
            file >> timeLimit;
        }
    }
    if(timeLimit < 0) timeLimit = DEFAULT_TIME_LIMIT;
    return timeLimit;
}

void Watchdog::cycle() {
    while (!kill)
    {
        if(getKick()) {
            this->startTime = clock();
        }
        setKick(false);
        usleep(50000);
    }
}

bool Watchdog::getKick() {
    bool res = false;
    p_kick = (bool*) shmat(shmidKick,(void*)0,SHM_RDONLY);
    res = *p_kick;
    shmdt(p_kick);
    return res;
}

void Watchdog::setKick(bool _kick) {
    p_kick = (bool*) shmat(shmidKick,(void*)0,0);
    *p_kick = _kick;
    shmdt(p_kick);
}

void Watchdog::setState(bool _state) {
    bool* shmState = (bool*) shmat(shmidState,(void*)0,0);
    *shmState = _state;
    shmdt(shmState);
}

bool Watchdog::readState() {
    bool* p_state = (bool*) shmat(shmidState,(void*)0,0);
    bool state = *p_state; 
    shmdt(p_state); 
    return state;
}


Watchdog test;
bool stop = false;

void signalHandler( int signum ) {
    cout << "Interrupt signal (" << signum << ") received.\n";
    
    test.stop();
    stop = true;
}

int main(){
    signal(SIGINT, signalHandler); 

    while(!stop) {
        cout << "wait for primary service v2" << endl;
        while(!test.getKick() && !stop)
            usleep(50000);
        
        if(!stop) {
            cout << "first kick" << endl;
            test.start();
            cout << "primary lost \n\n\n";
        }
    }
    cout << "ending" << endl;
}