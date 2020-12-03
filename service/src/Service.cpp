#include <stdio.h>
#include <iostream>
#include <stdlib.h>
#include <string>
#include <ctime>
#include <unistd.h>
#include <fstream>
#include <list>

using namespace std;

class Service {
    int width, height;
  public:
    int saveMemory(int);
    int loadMemory(int);
};

int Service::saveMemory (int x) {
    ofstream memory;
    memory.open("data/memory");
    memory << x;
    memory.close();
    width = x;
}

int loadMemory(int n){
    ifstream memory;
    memory.open("data/memory");
    
    string line;

    list<int> x (n);
    if(memory.is_open()){
        while ( getline(memory,line) )
        {
            cout << line << '\n';
        }
        memory.close();
    }
    return 0;
}


int main(int argc, char const *argv[])
{
	Service srv;
    int n = 5;
    srv.loadMemory(n);
	
    return 0;
}
