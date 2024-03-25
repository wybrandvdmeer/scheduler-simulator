#include <string>
#include <vector>

#include "Process.h"

#ifndef MEM_STRAT_H
#define MEM_STRAT_H

class MemStrategy {
public:
	MemStrategy();
	const int UPPER_MEM_LIMIT=2048;
	const int FRAME_SIZE=4;
	const int NO_OF_FRAMES=UPPER_MEM_LIMIT/FRAME_SIZE;
	virtual std::string getName();
	virtual int getMemUsage();
	virtual int allocatedAt(Process *process);
	virtual std::vector<int> * free(Process *process);
	virtual bool alloc(Process * process);	
	virtual std::string getMemFrames(Process * process);	
	virtual std::vector<std::vector<int>> * evict(std::vector<Process*> queue, Process * process);	
};

#endif
