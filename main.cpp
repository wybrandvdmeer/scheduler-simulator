#include <iostream>
#include <algorithm>
#include <string>

#include "Process.h"
#include "Scheduler.h"
#include "FirstFitMemStrategy.h"
#include "PagedMemStrategy.h"
#include "VirtualMemStrategy.h"

using namespace std;

char const *USAGE="allocate -f <filename> -q (1|2|3) -m (infinite | first-fit | paged | virtual)\n";

char* getOption(char ** begin, char ** end, const std::string & option) {
    char ** itr = std::find(begin, end, option);
    if (itr != end && ++itr != end)
    {
        return *itr;
    }
    return NULL;
}

MemStrategy * getMemStrategy(char * sMemStrategy) {
	std::string ms(sMemStrategy);
	if("infinite" == ms) {
		return NULL;
	}

	if("first-fit" == ms) {
		return new FirstFitMemStrategy();
	}
	
	if("paged" == ms) {
		return new PagedMemStrategy();
	}
	
	if("virtual" == ms) {
		return new VirtualMemStrategy();
	}

	cerr << USAGE;
	exit(1);	
}

int main(int argc, char ** argv) {
	char *file = getOption(argv, argv + argc, "-f");
	char *sQuantum = getOption(argv, argv + argc, "-q");
	char *memStrategy = getOption(argv, argv + argc, "-m");

	if(file == NULL || sQuantum == NULL || memStrategy == NULL) {
		cerr << USAGE;
		exit(1);
	}

	int quantum;
    try {
    	quantum = std::stoi(sQuantum);
    } catch(std::exception const &e) {
		cerr << USAGE;
		exit(1);
    }

	std::vector<Process*> processes = Process::readProcessFile(file);

	Scheduler scheduler(quantum, getMemStrategy(memStrategy), processes);
	scheduler.start();

	exit(0);
}
