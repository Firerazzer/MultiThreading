#include "Service.hpp"
#include <chrono>
#define n 5

Service::Service(uint16_t _port, int protocole) {
    this->protocole = protocole;
    this->port = _port;

    shmidKick = shmget(12345,sizeof(bool),0666|IPC_CREAT);
    shmidState = shmget(12346,sizeof(bool),0666|IPC_CREAT);

    //Lance la lecture du capteur si on est en primary
    if(this->protocole == PRIMARY)
        driver.start(this->port);

    //lance le cycle du service
    killCycle = false;
    this->threadCycle = thread(&Service::cycle, this);
}

Service::~Service() {
    killCycle = true;
    if(threadCycle.joinable())
        threadCycle.join();
    driver.stop();
}

int Service::saveMemory () {
    ofstream memory("service/data/memory", fstream::trunc);
    string valueToSave;
    
    if(memory.is_open()){
        for(int x : this->storage) {
            valueToSave = "\n" + std::to_string(x);
            memory << valueToSave;
        }
        memory.close();
        return 1;
    }
    else
    {
        cerr << "Le fichier n'a pas pu être ouvert!" << '\n';
        return -1;
    }
    


    return 0;
}

void Service::loadMemory(){
    // auto start = std::chrono::high_resolution_clock::now();
    ifstream memory ("service/data/memory");
    string line;

    this->storage.clear();

    if(memory.is_open()){
        getline(memory,line);
        while ( getline(memory,line))
        {
            int value = stoi(line);
            this->storage.push_back(value);
        }
        memory.close();
    }
    else
    {
        cerr << "Le fichier n'a pas pu être ouvert!" << '\n';
        cerr << "Demarrage sans recuperation de la mémoire stable" << '\n';
    }

    // std::array<char, 128> buffer;
    // std::unique_ptr<FILE, decltype(&pclose)> pipe(popen("tail -5 service/data/memory", "r"), pclose);
    // if (!pipe) {
    //     throw std::runtime_error("popen() failed!");
    // }
    // while (fgets(buffer.data(), buffer.size(), pipe.get()) != nullptr) {
    //     this->storage.push_back(atoi(buffer.data()));
    // }

    // auto finish = std::chrono::high_resolution_clock::now();
    // std::chrono::duration<double> elapsed = finish - start;
    // std::cout << "Elapsed time: " << elapsed.count() << " s\n";
}

double Service::calculOutput() {
    double sum = 0;
    for(int nb : this->storage) {
        sum += nb;
    }

    //si le mode erreur est active
    if(error_mode > 0) {
        //on genere une erreur en valeure 1 fois/2
        if(error_mode%2 != 0){
            sum+=6;
        }
        //a la 10eme demande on genere encore une erreur pour avoir 2 fautes d'affilees
        if(error_mode == 10)
            sum+=33;
        error_mode++;
    }
    return sum/this->storage.size();
}

void Service::display(int result) {
    cout << "Result : " << result << "\n";
}

bool Service::readStateWatchdog() {
    bool* p_state = (bool*) shmat(shmidState,(void*)0,0);
    bool state = *p_state; 
    shmdt(p_state); 
    return state;
}

void Service::cycle() {
    while (!killCycle)
    {
        // Processing in PRIMARY mode
        if(this->protocole == Protocole::PRIMARY) {
            kickWatchdog();
            //cout << this->driver.dataReceived.size() << endl;
            if(this->driver.dataReceived.size() > 0) {
                this->storage.push_back(atoi(this->driver.dataReceived.front().c_str()));
                this->driver.dataReceived.pop();
                while(this->storage.size() > n)
                    this->storage.pop_front();
                double res1 = calculOutput();
                double res2 = calculOutput();
                if(res1 != res2) {
                    cout << "Erreur de valeur, tentative de vote majoritaire" << endl;
                    double res3 = calculOutput();
                    if(res3 == res1)
                        cout << "output : " << res1 << endl;
                    else if (res3 == res2)
                        cout << "output : " << res2 << endl;
                    else {
                        cerr << "Echec du vote majoritaire !" << endl;
                        cerr << "Arret du service !" << endl;
                        killCycle = true;
                    }
                } else
                    cout << "output : " << res1 << endl;
                
                if(saveMemory() < 1)
                    cerr << "Erreur sauvegarde" << endl;
            }

        // Processing in BACKUP mode
        } else {
            bool primaryAlive = readStateWatchdog();
            if(!primaryAlive) {
                usleep(100);
                if(!readStateWatchdog()) {
                    loadMemory();
                    this->protocole = Protocole::PRIMARY;
                    cout << "Switch BACKUP to PRIMARY" << endl;
                    cout << "value loaded from memory : " << this->calculOutput() << endl;
                    //demarrage de la lecture du capteur
                    driver.start(this->port);
                }
            }
        }
        usleep(50000);
    }
}

void Service::  kickWatchdog() {
    this->p_kick = (bool*) shmat(shmidKick,(void*)0,0);
    *p_kick = true; 
    shmdt(p_kick); 
}

int main(int argc, char const *argv[])
{
    //auto start = std::chrono::high_resolution_clock::now();
    Protocole pr;
    if(argc <= 1)
	    pr = PRIMARY;
    else
        if(strcmp(argv[1], "backup") == 0)
	        pr = BACKUP;
        else
            pr = PRIMARY;

    //srv.loadMemory();
    //auto finish = std::chrono::high_resolution_clock::now();
    //std::chrono::duration<double> elapsed = finish - start;
    //std::cout << "Elapsed time: " << elapsed.count() << " s\n";

    Service srv(pr == PRIMARY ? 8080 : 8081, pr);
    if(argc >= 3) {
        srv.error_mode = 1;
        cout << "mode erreur active" << endl;
    }
	sleep(pr == PRIMARY ? 5 : 10);
    srv.display(srv.calculOutput());
    return 0;
}