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
  public:
    int saveMemory(int x);
    list<int> loadMemory(int n);
};

int Service::saveMemory (int x) {
    ofstream memory;
    string valueToSave;

    valueToSave = "\n" + std::to_string(x);

    memory.open("service/data/memory", fstream::app);
    
    if(memory.is_open()){
        memory << valueToSave;
        memory.close();
        return 1;
    }
    else
    {
        cout << "Le fichier n'a pas pu être ouvert!" << '\n';
        return -1;
    }
    


    return 0;
}

list<int> Service::loadMemory(int n){
    ifstream memory ("service/data/memory");
    string line;

    list<int> valueLoaded;
    list<int>::iterator it = valueLoaded.begin();

    int i =0;

    if(memory.is_open()){
        while ( getline(memory,line) && i != n)
        {
            i++;
            int value = stoi(line);
            valueLoaded.push_back(value);
            it++;
            cout << *it << '\n';

        }
        memory.close();
    }
    else
    {
        cout << "Le fichier n'a pas pu être ouvert!" << '\n';
        valueLoaded.push_back(-1);
    }
    
    return valueLoaded;
}


int main(int argc, char const *argv[])
{
	Service srv;
    int n = 11;
    srv.saveMemory(n);
    srv.loadMemory(n);
	
    return 0;
}
