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
        int saveMemory(int x);
        void loadMemory();
        double calculOutput();
        bool readStateWatchdog();
        Service(uint16_t _port, int protocole);
        ~Service();
        void display(int result);

    private:
        list<int> storage;
        bool killCycle;
        thread threadCycle;
        bool *p_kick;
        int shmidKick;
        int shmidState;
        CapteurDriver driver;
        int protocole;

        void cycle();
        void kickWatchdog();
};