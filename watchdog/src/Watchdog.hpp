#include <atomic>
#include <fstream>
#include <iostream>

class Watchdog{
    private:
    bool stateSys;
    int secondes;
    std::atomic<unsigned int> _timer;
    bool kill;

    public:
    Watchdog();
    ~Watchdog(){};
    bool kick();
    int getTimeLimit();
};