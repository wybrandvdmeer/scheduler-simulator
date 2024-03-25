#include <chrono>
#include <thread>
#include <queue>
#include <iostream>

#include "Process.h"
#include "Scheduler.h"

using namespace std;

Scheduler::Scheduler(int quantum, std::queue<Process> processes) {
	Scheduler::quantum = quantum;
	Scheduler::processes = processes;
}

void Scheduler::start() {
	unsigned int time=0;
	do {
		add2Scheduler(time);	

		runningProcess = pop();
		runningProcess.state = RUNNING;

		print(time, runningProcess);

		/* Processing. */

		State state = runningProcess.update(quantum);
		if(state == READY) {
			queue.push(runningProcess);
		}

		if(state == FINISHED) {
			// Deallocate memory.
		}

		time+=quantum;
	} while(Scheduler::queue.size() > 0);
}

void Scheduler::add2Scheduler(unsigned int time) {
	while(!processes.empty()) {
		Process p = processes.front();
		if(p.time <= time) {
			queue.push(p);
			processes.pop();
		} else { 
			return;
		}
	} 
}

Process Scheduler::pop() {
	Process p = queue.front();
	queue.pop();
	return p;
}

void Scheduler::print(unsigned int time, Process process) {
	char buf[1000];
	sprintf(buf,"%u,%s,process-name=%s,remaining-time=%d,mem-usage=%u,allocated-at=%u,mem-frames=%u\n", 
		time,
		printStatus(process.state),
		process.name.c_str(),
		process.remainingTime,
		process.memory,
		(unsigned int)0, (unsigned int)0);
		
	cout << buf << std::flush;
}

const char * Scheduler::printStatus(State state) {
	return state == READY ? "READY" : 
		(state == FINISHED ? "FINISHED" : 
			(state == RUNNING ? "RUNNING" : "unknown"));
}
