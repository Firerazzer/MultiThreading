#include "Service.hpp"

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
    int i = 0;
    while(i < 14) {
        if(srv.getProtocole() == PRIMARY) {
            usleep(500000);
            cout << "result : " << srv.getValue() << endl;
            i++;
        }
    }
    return 0;
}