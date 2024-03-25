#include <string>
#include <vector>

#include "MemStrategy.h"
#include "Process.h"

class Scheduler {
public:
	Scheduler(int quantum, MemStrategy * memStrategy, std::vector<Process*> processes);
	void start();
private:
	int quantum;
	unsigned int noOfProcesses=0;
	float maxOverheadFactor=0.0, summedOverheadFactor=0.0, summedTurnedAroundTime=0.0;
	std::vector<Process*> processes, queue;
	MemStrategy * memStrategy;

	void add2Scheduler(unsigned int time);
	Process * pop();
	void print(unsigned int time);
	void print(unsigned int time, std::vector<int> frames);
	void print(unsigned int time, Process *process);
	const char * printStatus(State state);
	void updateStatistics(unsigned int time, Process *process);
};
