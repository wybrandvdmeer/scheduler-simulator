#include <set>
#include <vector>
#include <cmath>
#include <algorithm>
#include <iostream>

#include "Process.h"
#include "FirstFitMemStrategy.h"

using namespace std;

std::vector<int> * FirstFitMemStrategy::free(Process * process) {
	auto it = allocs.find(process);
	if(it != allocs.end()) {
		allocs.erase(it);
	}
	return NULL;
}

std::vector<std::vector<int>> * 
	FirstFitMemStrategy::evict(std::vector<Process*> queue, Process * process) {
	return NULL;
}

std::string FirstFitMemStrategy::getName() {
	return "first-fit";
}

int FirstFitMemStrategy::allocatedAt(Process *process) {
	auto it = allocs.find(process);
	if(it != allocs.end()) {
		return it->second.first;
	}
	return -1;
}

int FirstFitMemStrategy::getMemUsage() {
	int totalMemory=0;
	for(std::map<Process*,std::pair<int,int>>::iterator it = allocs.begin(); 
		it != allocs.end(); ++it) {
		totalMemory += (it->second.second - it->second.first); 
	}

	return (int)std::round((((float)totalMemory)/UPPER_MEM_LIMIT) * 100);
}

bool FirstFitMemStrategy::alloc(Process * process) {
	if(allocs.count(process)) {
		return true;
	}

	std::set<std::pair<int,int>, cmpStr> allocatedPairs;

	for(std::map<Process*,std::pair<int,int>>::iterator it = allocs.begin(); it != allocs.end(); ++it) {
		allocatedPairs.insert(it->second);
	}

	std::pair<int,int> mem = allocPair(process->memory, allocatedPairs);
	if(mem.second - mem.first > 0) {
		allocs[process] = mem;
		return true;
	}

	return false;
}

std::pair<int,int> FirstFitMemStrategy::allocPair(int size, std::set<std::pair<int,int>, FirstFitMemStrategy::cmpStr> allocatedPairs) {
	int lower=0;
	for(auto p : allocatedPairs) {
		if(lower < p.first) {
			if(p.first - 1 - lower <= size) {
				return std::make_pair(lower, lower + size);
			}
		}
		lower = p.second;
	}

	if(lower < UPPER_MEM_LIMIT && size <= UPPER_MEM_LIMIT - lower) {
		return std::make_pair(lower, lower + size);
	}

	return std::make_pair(0,0);
}

std::string FirstFitMemStrategy::getMemFrames(Process * process) {
	return "";
}

















