#include <stdio.h>
#include <iostream>
#include <stdlib.h>
#include <ctime>
#include <unistd.h>

using namespace std;

int main(int argc, char const *argv[])
{
	srand(117);
	while (true)
	{
		printf("%d\n", rand() %100);
		sleep(1);
	}
	
    return 0;
}
