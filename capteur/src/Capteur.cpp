#include <stdio.h>
#include <iostream>
#include <stdlib.h>
#include <ctime>
#include <unistd.h>

using namespace std;

int main(int argc, char const *argv[])
{
	while (true)
	{
		printf("%d\n", rand() %100);
		sleep(1);
	}
	
    return 0;
}
