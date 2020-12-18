#include "Watchdog.hpp"
#include <string.h>

Watchdog::Watchdog(){
    stateSys = false;
    _timer = 0; //exprimé en 10^-5 secondes
    kill = false;

    // ftok to generate unique key 
    key_t key = ftok("shmkick",65); 
    // shmget returns an identifier in shmid 
    shmid = shmget(key,sizeof(bool),0666|IPC_CREAT); 
}

Watchdog::~Watchdog(){
    stateSys = false;

    // destroy the shared memory 
    shmctl(this->shmid,IPC_RMID,NULL);       
}

bool Watchdog::start(){
    while(stateSys){
        _timer++;
        if(_timer > getTimeLimit()){
            stateSys = false;
            kill = true;
        }
    }
    return kill; 
}

int Watchdog::getTimeLimit(){
    int nbSecond;
    std::ifstream file ("save.txt");
    if(!file){
        std::cerr << "Impossible d'ouvrir le fichier " << std::endl;
        return 1;
    } else{
        file >> nbSecond;
    }
    return nbSecond;
}

void Watchdog::cycle() {
    while (!kill)
    {
        // shmat to attach to shared memory 
        p_kick = (bool*) shmat(shmid,(void*)0,0);
        if(*p_kick) {
            this->_timer = 0;
            stateSys = true;
        }
        *p_kick = false;
        shmdt(p_kick);
        usleep(50000);
    }
    
}


int main(){
    //detach from shared memory  
    Watchdog test;
    while(!test.stateSys)
        usleep(50000);
        
    if(test.start()){
        std::cout << "KILL" << std::endl;
    };
}