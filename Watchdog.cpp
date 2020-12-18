#include "Watchdog.h"

Watchdog::Watchdog(){
    stateSys = true;
    _timer = 0; //exprimé en 10^-5 secondes
    kill = false;
}

bool Watchdog::kick(){
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


int main(){
    Watchdog test;
    if(test.kick()){
        std::cout << "KILL" << std::endl;
    };
}