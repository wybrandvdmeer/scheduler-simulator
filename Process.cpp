#include <queue>
#include <string>
#include <fstream>
#include <iostream>

#include "Process.h"

using namespace std;

std::queue<Process> Process::readProcessFile(char *f) {
	ifstream file;
	string name;
 	int time, remainingTime, memory;

	std::queue<Process> processes;
	
	file.open(f);

	while(file >> time >> name >> remainingTime >> memory) {
		Process * process = new Process();
		process->name = name;		
		process->time = time;		
		process->remainingTime = remainingTime;		
		process->memory = memory;
		processes.push(*process);		
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
