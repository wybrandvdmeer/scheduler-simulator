#include <cmath>
#include <thread>
#include <vector>
#include <iostream>
#include <cstring>

#include "Process.h"
#include "Scheduler.h"

using namespace std;

Scheduler::Scheduler(int quantum, MemStrategy * memStrategy, std::vector<Process*> processes) {
	Scheduler::quantum = quantum;
	Scheduler::processes = processes;
	Scheduler::memStrategy = memStrategy;
}

void Scheduler::start() {
	unsigned int time=0;
	Process *proc, *prvProc=NULL;
	
	add2Scheduler(time);	
	do {
		proc = pop();

		if(proc == NULL) {
			time+=quantum;
			add2Scheduler(time);
			continue;
		}

		if(memStrategy != NULL && !memStrategy->alloc(proc)) {
			std::vector<std::vector<int>> * frames=memStrategy->evict(queue, proc);
			if(frames != NULL) {
				for(std::vector<std::vector<int>>::iterator it = frames->begin(); 
				it != frames->end(); it++) {
					print(time, *it);
				}

				delete frames;

				if(!memStrategy->alloc(proc)) {
					cout << "Cannot run process even after evicting memory of other processes.";
					exit(1);
				}
			} else {
				queue.push_back(proc);
				prvProc = proc; // Suspect, but necessary to get the same output.
				continue;
			}
		}

		proc->state = RUNNING;

		if(prvProc != proc) {
			print(time, proc);
			prvProc = proc;
		}

		time+=quantum;

		add2Scheduler(time);

		State state = proc->update(quantum);
		if(state == READY) {
			queue.push_back(proc);
		}

		if(state == FINISHED) {
			updateStatistics(time, proc);
			if(memStrategy != NULL) {
				std::vector<int> * frames = memStrategy->free(proc);
				if(frames != NULL) {
					print(time, *frames);
					delete frames;
				}	
			}
			print(time, proc);

			delete proc;
		}
	} while(queue.size() > 0 || processes.size() > 0);

	print(time);
}

void Scheduler::updateStatistics(unsigned int time, Process *process) {
	noOfProcesses++;

	summedTurnedAroundTime += (time - process->time);

	float overheadFactor = (float)(time - process->time) / process->serviceTime;
	if(maxOverheadFactor < overheadFactor) {
		maxOverheadFactor = overheadFactor;
	}

	summedOverheadFactor += overheadFactor;
}

void Scheduler::add2Scheduler(unsigned int time) {
	while(!processes.empty()) {
		Process * p = processes.front();
		if(p->time <= time) {
			queue.push_back(p);
			processes.erase(processes.begin());
		} else { 
			return;
		}
	} 
}

void Scheduler::print(unsigned int time) {
	char buf[1000];
	memset(buf, 0x00, 1000 * sizeof(char));
	sprintf(buf, "Turnaround time %.0f\n", std::ceil(summedTurnedAroundTime / noOfProcesses)); 
	sprintf(buf + strlen(buf), 
		"Time overhead %.2f %.2f\n", maxOverheadFactor, summedOverheadFactor / noOfProcesses);
	sprintf(buf + strlen(buf), "Makespan %u\n", time);
	cout << buf;
}

void Scheduler::print(unsigned int time, vector<int> frames) {
	char buf[6000];
	memset(buf, 0x00, 6000 * sizeof(char));
	sprintf(buf, "%u,EVICTED,evicted-frames=", time);

	std::string sFrames;
    for(std::vector<int>::iterator it=frames.begin(); it != frames.end(); it++) {
		sFrames += std::to_string(*it);
       	if(it != frames.end() - 1) {
        	sFrames += ",";
        }
	}

	sprintf(buf + strlen(buf), "[%s]\n", sFrames.c_str());
	cout << buf;
}

void Scheduler::print(unsigned int time, Process *process) {
	char buf[6000];
	memset(buf, 0x00, 6000 * sizeof(char));
	sprintf(buf, "%u,%s,process-name=%s,", 
		time,
		printStatus(process->state),
		process->name.c_str());

	if(process->state == FINISHED) {	
		sprintf(buf+strlen(buf), "proc-remaining=%ld", queue.size());
	} else {
		sprintf(buf+strlen(buf), "remaining-time=%d", process->remainingTime);
	} 

	if(process->state != FINISHED && memStrategy != NULL) {	
		if(memStrategy->getName() == "first-fit") {
			sprintf(buf + strlen(buf), 
				",mem-usage=%d%c,allocated-at=%d", 
				memStrategy->getMemUsage(),
				'%',
				memStrategy->allocatedAt(process)); 
		} else {
			sprintf(buf + strlen(buf), 
				",mem-usage=%d%c,mem-frames=%s", 
				memStrategy->getMemUsage(),
				'%',
				memStrategy->getMemFrames(process).c_str());
		}
	}

	buf[strlen(buf)] = '\n';

	cout << buf << std::flush;
}

const char * Scheduler::printStatus(State state) {
	return state == READY ? "READY" : 
		(state == FINISHED ? "FINISHED" : 
			(state == RUNNING ? "RUNNING" : "unknown"));
}

Process * Scheduler::pop() {
	if(queue.size() == 0) {
		return NULL;
	}

	Process * p = queue.front();
	queue.erase(queue.begin());
	return p;
}

