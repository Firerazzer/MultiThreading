#include <atomic>
#include <fstream>
#include <iostream>
#include <string>
#include <unistd.h>
#include <thread>
#include <sys/ipc.h> 
#include <sys/shm.h> 

using namespace std;

class Watchdog{
    private:
    int secondes;
    std::atomic<unsigned int> _timer;
    bool kill, *p_kick;
    thread threadCycle;
    int shmid;
    void cycle();

    public:
    bool stateSys;
    Watchdog();
    ~Watchdog(){};
    bool start();
    int getTimeLimit();
};