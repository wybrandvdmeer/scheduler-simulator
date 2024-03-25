#include <string>
#include <vector>

#include "State.h"

#ifndef PROCESS_H
#define PROCESS_H

class Process {
public:
	std::string name;
	unsigned int time;
	int serviceTime, remainingTime, memory;

	State state=READY;

	Process();
	Process(std::string name, unsigned int time, int serviceTime, int memory);
	static std::vector<Process *> readProcessFile(char *file);
	State update(int quantum);

	bool operator <(const Process& rhs) const;
};

#endif
