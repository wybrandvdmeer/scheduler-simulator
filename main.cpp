#include <iostream>
#include <algorithm>
#include <string>

#include "Process.h"
#include "Scheduler.h"

using namespace std;

char const *USAGE="allocate -f <filename> -q (1|2|3)\n";

char* getOption(char ** begin, char ** end, const std::string & option) {
    char ** itr = std::find(begin, end, option);
    if (itr != end && ++itr != end)
    {
        return *itr;
    }
    return NULL;
}

int main(int argc, char ** argv) {
	char *file = getOption(argv, argv + argc, "-f");
	char *sQuantum = getOption(argv, argv + argc, "-q");

	if(file == NULL || sQuantum == NULL) {
		cerr << USAGE;
		return 1;
	}

	int quantum;
    try {
    	quantum = std::stoi(sQuantum);
    } catch(std::exception const &e) {
		cerr << USAGE;
    	return 1;
    }

	std::queue<Process> processes = Process::readProcessFile(file);

	Scheduler scheduler(quantum, processes);
	scheduler.start();

    return 0;
}
