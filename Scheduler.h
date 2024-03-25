#include <string>
#include <queue>

#include "Process.h"

class Scheduler {
public:
	Scheduler(int quantum, std::queue<Process> processes);
	void start();
private:
	std::queue<Process> processes, queue;
	Process runningProcess;
	int quantum;
	void add2Scheduler(unsigned int time);
	void print(unsigned int time, Process process);
	const char * printStatus(State state);
	Process pop();
};
