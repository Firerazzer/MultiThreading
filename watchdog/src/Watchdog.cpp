#include "Watchdog.hpp"
#include <string.h>

Watchdog::Watchdog(){
    stateSys = false;
    startTime = clock();
    kill = false;

    // ftok to generate unique key 
    key_t keyKick = ftok("shmkick",65); 
    key_t keyState = ftok("shmstate",65); 
    // shmget returns an identifier in shmid 
    shmidKick = shmget(keyKick,sizeof(bool),0666|IPC_CREAT); 
    shmidState = shmget(keyState,sizeof(bool),0666|IPC_CREAT); 
    cout << "shmidState : " << shmidState << endl;
    setState(false);
}

Watchdog::~Watchdog(){
    stateSys = false;
    kill = true;
    threadCycle.join();

    // destroy the shared memory 
    shmctl(this->shmidKick,IPC_RMID,NULL);       
    shmctl(this->shmidState,IPC_RMID,NULL); 
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
            //setState(false);
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
        cout << "ShState : " << readState() << endl;
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


int main(){
    Watchdog test;

    while(true) {
        cout << "wait for primary service v2" << endl;
        while(!test.getKick())
            usleep(50000);
        
        cout << "first kick" << endl;
        test.start();
        cout << "primary lost \n\n\n";
    }
}