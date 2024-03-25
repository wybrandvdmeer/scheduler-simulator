#include <vector>
#include <string>
#include <fstream>
#include <iostream>

#include "Process.h"

using namespace std;

Process::Process() {
}

Process::Process(std::string name, unsigned int time, int serviceTime, int memory) {
	Process::name = name;
	Process::time = time;
	Process::serviceTime = serviceTime;
	Process::remainingTime = serviceTime;
	Process::memory = memory;
}

std::vector<Process *> Process::readProcessFile(char *f) {
	ifstream file;
	string name;
 	int time, serviceTime, memory;

	std::vector<Process*> processes;
	
	file.open(f);

	while(file >> time >> name >> serviceTime >> memory) {
		Process * process = new Process(name, time, serviceTime, memory);
		processes.push_back(process);		
	}

	file.close();

	return processes;
}

State Process::update(int quantum) {
	if(state == RUNNING) {
		remainingTime -= quantum;
		if(remainingTime <= 0) {
			state = FINISHED;	
		} else {
			state = READY;
		}
	} else {
		throw std::invalid_argument("Can only update processes which are running.");
	}

	return state;
}

bool Process::operator <(const Process& rhs) const {
	return this->name.compare(rhs.name) < 0;
}
