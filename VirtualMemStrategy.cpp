#include <set>
#include <vector>
#include <cmath>
#include <algorithm>
#include <iostream>

#include "Process.h"
#include "VirtualMemStrategy.h"

using namespace std;

std::vector<int> * VirtualMemStrategy::free(Process * process) {
	std::vector<int> * frames=NULL;
	auto it = allocs.find(process);
    if(it != allocs.end()) {
		frames = it->second;
       	allocs.erase(it);
    }
	return frames;
}

std::vector<std::vector<int>> * 
	VirtualMemStrategy::evict(std::vector<Process*> queue, Process * process) {
	int noOfFrames = std::min((int)std::ceil(((float)process->memory)/FRAME_SIZE), 
		MINIMAL_NO_OF_ALLOCATED_PAGES);

	int noOfFreeFrames = NO_OF_FRAMES - getAllocatedFrames().size();

	std::vector<Process*> evictedProcesses;
	std::vector<int> frames; 

    for(std::vector<Process*>::iterator it = queue.begin(); it != queue.end() &&
        noOfFreeFrames < noOfFrames; ++it) {
        for(auto a : allocs) {
            if(a.first == *it) {

				for(std::vector<int>::iterator itt = a.second->begin(); 
					itt != a.second->end() && noOfFreeFrames < noOfFrames; 
					itt++, noOfFreeFrames++) {
					frames.push_back(*itt);
				}

				for(auto f : frames) {
					a.second->erase(std::find(a.second->begin(), a.second->end(), f));
				}

				if(a.second->empty()) {
					evictedProcesses.push_back(*it);
				}	
            }
        }
    }

	if(noOfFrames <= noOfFreeFrames) {
		for(auto p : evictedProcesses) {
			allocs.erase(p);
		}

		std::vector<std::vector<int>> * c = new std::vector<std::vector<int>>();
		c->push_back(frames);
		return c;
	}

	return NULL;
}

std::string VirtualMemStrategy::getName() {
	return "virtual";
}

int VirtualMemStrategy::allocatedAt(Process *process) {
	return -1;
}

int VirtualMemStrategy::getMemUsage() {
	return (int)std::ceil((((float)getAllocatedFrames().size())/NO_OF_FRAMES) * 100);
}

bool VirtualMemStrategy::alloc(Process * process) {
    if(allocs.count(process)) {
        return true;
    }

	int noOfFrames = std::ceil(((float)process->memory)/FRAME_SIZE);

	std::vector<int> * reserved = allocate(noOfFrames, getAllocatedFrames());

	if(MINIMAL_NO_OF_ALLOCATED_PAGES <= reserved->size()) {
		allocs[process] = reserved;
		return true;
	}

	return false;
}

vector<int> VirtualMemStrategy::getAllocatedFrames() {
	std::vector<int> frames;
    for(std::map<Process*,std::vector<int> *>::iterator it = allocs.begin(); 
		it != allocs.end(); ++it) {
		for(std::vector<int>::iterator itt = it->second->begin();
			itt != it->second->end(); itt++) {
			frames.push_back(*itt);
		}
    }
	return frames;
}

std::vector<int> * VirtualMemStrategy::allocate(int noOfFrames, vector<int> v) {
	std::vector<int> * reserved = new std::vector<int>();
	for(int idx=0; idx < NO_OF_FRAMES && reserved->size() < noOfFrames; idx++) {
		if(std::find(v.begin(), v.end(), idx) == v.end()) {
			reserved->push_back(idx);
		}
	}
	return reserved;
}

std::string VirtualMemStrategy::getMemFrames(Process * process) {
	std::string frames="[";
    auto it = allocs.find(process);
    if(it != allocs.end()) {
		for(std::vector<int>::iterator itt = it->second->begin(); itt != it->second->end(); itt++) {
			frames += std::to_string(*itt);
			if(itt != it->second->end() - 1) {
				frames += ",";
			} 
		}
    }

    return frames + "]";
}

















