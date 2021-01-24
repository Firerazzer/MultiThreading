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
#include <atomic>
#include "CapteurDriver.hpp"
#include "Protocole.hpp"

using namespace std;

class Service {
    public:
        bool killCycle;
        char error_mode = false;
        
        double getValue();
        int getProtocole();
        Service(uint16_t _port, int protocole);
        ~Service();

    private:
        list<int> storage;
        thread threadCycle;
        bool *p_kick;
        int shmidKick;
        int shmidState;
        CapteurDriver driver;
        uint16_t port;
        int protocole;
        std::atomic<double> value;

        void cycle();
        void kickWatchdog();
        int saveMemory();
        void loadMemory();
        bool readStateWatchdog();
        double calculOutput();
};