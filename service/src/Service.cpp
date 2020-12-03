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
        int calculOutput(list<int> list);
        void display(int result);

    private:
        list<int> storage;
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
        storage.push_back(-1);
    }

    return valueLoaded;
}

int Service::calculOutput(list<int> list) {
    int i = 0;
    for(int nb : list) {
        if(i == list.size()/2) {
            return nb;
        }
        i++;
    }
}

void Service::display(int result) {
    cout << "Result : " << result << "\n";
}

int main(int argc, char const *argv[])
{
	Service srv;
    int n = 5;
    srv.saveMemory(n);
    list<int> l = srv.loadMemory(n);
    int r = srv.calculOutput(l);
    srv.display(r);
	
    return 0;
}