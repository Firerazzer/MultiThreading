#include <atomic>
#include <fstream>
#include <iostream>
#include <string>
#include <unistd.h>
#include <thread>
#include <sys/ipc.h> 
#include <sys/shm.h> 
#include <ctime>
#include <csignal>

using namespace std;

#define DEFAULT_TIME_LIMIT 75000

class Watchdog{
    private:
    int secondes, timeLimit = -1;
    std::atomic<clock_t> startTime;
    std::chrono::duration<double> elapsed;
    bool kill, *p_kick;
    thread threadCycle;
    int shmidKick, shmidState;
    void watch();
    void cycle();
    int getTimeLimit();

    public:
    bool stateSys;
    Watchdog();
    ~Watchdog();
    void start();
    void stop();
    bool getKick();
    void setKick(bool _kick);
    void setState(bool _state);
    bool readState();
};