#include <string>
#include <queue>

#include "State.h"

#ifndef PROCESS_H
#define PROCESS_H

class Process {
public:
	std::string name;
	unsigned int time;
	int remainingTime;
	unsigned int memory;

	State state=READY;

	static std::queue<Process> readProcessFile(char *file);

	State update(int quantum);
};

#endif
