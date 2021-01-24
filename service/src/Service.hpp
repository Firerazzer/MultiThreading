#include <stdio.h>
#include <iostream>
#include <stdlib.h>
#include <string>
#include <ctime>
#include <sys/ipc.h> 
#include <sys/shm.h> 
#include <unistd.h>
#include <fstream>
#include <list>
#include <thread>
#include "CapteurDriver.hpp"
#include "Protocole.hpp"

using namespace std;

class Service {
    public:
        bool killCycle;
        char error_mode = false;
        
        int saveMemory();
        void loadMemory();
        double calculOutput();
        bool readStateWatchdog();
        Service(uint16_t _port, int protocole);
        ~Service();
        void display(int result);

    private:
        list<int> storage;
        thread threadCycle;
        bool *p_kick;
        int shmidKick;
        int shmidState;
        CapteurDriver driver;
        int protocole;
        uint16_t port;

        void cycle();
        void kickWatchdog();
};