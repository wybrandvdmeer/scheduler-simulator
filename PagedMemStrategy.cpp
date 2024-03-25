#include <set>
#include <vector>
#include <cmath>
#include <algorithm>
#include <iostream>

#include "Process.h"
#include "PagedMemStrategy.h"

using namespace std;

std::vector<int> * PagedMemStrategy::free(Process * process) {
	std::vector<int> * frames=NULL;
	auto it = allocs.find(process);
    if(it != allocs.end()) {
		frames = it->second;
       	allocs.erase(it);
    }
	return frames;
}

std::vector<std::vector<int>> * PagedMemStrategy::evict(std::vector<Process*> queue, Process * process) {
	int noOfFrames = std::ceil(((float)process->memory)/FRAME_SIZE);
	int noOfFreeFrames = NO_OF_FRAMES - getAllocatedFrames().size();

	std::vector<Process*> evictedProcesses;
	std::vector<std::vector<int>> * frames = new std::vector<std::vector<int>>();

    for(std::vector<Process*>::iterator it = queue.begin(); it != queue.end() &&
		noOfFreeFrames < noOfFrames; ++it) {
		for(auto a : allocs) {
			if(a.first == *it) {
				noOfFreeFrames += a.second->size();
				evictedProcesses.push_back(*it);
				frames->push_back(*a.second);
			}
		}
	}

	if(noOfFrames <= noOfFreeFrames) {
		for(auto p : evictedProcesses) {
			allocs.erase(p);
		}

		return frames;
	}

	return NULL;
}

std::string PagedMemStrategy::getName() {
	return "paged";
}

int PagedMemStrategy::allocatedAt(Process *process) {
	return -1;
}

int PagedMemStrategy::getMemUsage() {
	return (int)std::ceil((((float)getAllocatedFrames().size())/NO_OF_FRAMES) * 100);
}

bool PagedMemStrategy::alloc(Process * process) {
    if(allocs.count(process)) {
        return true;
    }

	std::vector<int> frames = getAllocatedFrames();

	int noOfFrames = std::ceil(((float)process->memory)/FRAME_SIZE);

	std::vector<int> * reserved = allocate(noOfFrames, frames);
	if(noOfFrames == reserved->size()) {
		allocs[process] = reserved;
		return true;
	}

	return false;
}

vector<int> PagedMemStrategy::getAllocatedFrames() {
	std::vector<int> frames;
    for(std::map<Process*,std::vector<int>*>::iterator it = allocs.begin(); 
		it != allocs.end(); ++it) {
		for(std::vector<int>::iterator itt = it->second->begin();
			itt != it->second->end(); itt++) {
			frames.push_back(*itt);
		}
    }
	return frames;
}

std::vector<int> * PagedMemStrategy::allocate(int noOfFrames, vector<int> v) {
	std::vector<int> * reserved = new std::vector<int>();
	for(int idx=0; idx < NO_OF_FRAMES && reserved->size() < noOfFrames; idx++) {
		if(std::find(v.begin(), v.end(), idx) == v.end()) {
			reserved->push_back(idx);
		}
	}
	return reserved;
}

std::string PagedMemStrategy::getMemFrames(Process * process) {
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

