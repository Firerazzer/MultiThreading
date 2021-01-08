#include <atomic>
#include <fstream>
#include <iostream>
#include <string>
#include <unistd.h>
#include <thread>
#include <sys/ipc.h> 
#include <sys/shm.h> 
#include <ctime>

using namespace std;

#define DEFAULT_TIME_LIMIT 75000

class Watchdog{
    private:
<<<<<<< HEAD
    int secondes;
    std::atomic<unsigned int> _timer;
    bool kill, *p_kick, *p_state;
    thread threadCycle;
    int shmidKick;
    int shmidState;
=======
    int secondes, timeLimit = -1;
    std::atomic<clock_t> startTime;
    std::chrono::duration<double> elapsed;
    bool kill, *p_kick;
    thread threadCycle;
    int shmidKick, shmidState;
    void watch();
>>>>>>> main
    void cycle();
    int getTimeLimit();

    public:
    bool stateSys;
    Watchdog();
    ~Watchdog();
<<<<<<< HEAD
    bool start();
    int getTimeLimit();
=======
    void start();
    bool getKick();
    void setKick(bool _kick);
    void setState(bool _state);
>>>>>>> main
};