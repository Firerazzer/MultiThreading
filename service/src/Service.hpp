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

using namespace std;

class Service {
    public:
        int saveMemory(int x);
        void loadMemory(int n);
        double calculOutput();
        Service(uint16_t _port);
        ~Service();
        void display(int result);

    private:
        list<int> storage;
        bool killCycle;
        thread threadCycle;
        bool *p_kick;
        int shmid;
        CapteurDriver driver;
        void cycle();
        void kickWatchdog();
};