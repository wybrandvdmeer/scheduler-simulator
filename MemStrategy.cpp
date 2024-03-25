#include <string>
#include <vector>

#include "MemStrategy.h"

using namespace std;

MemStrategy::MemStrategy() {
}

int MemStrategy::getMemUsage() {
	return 0;
}

int MemStrategy::allocatedAt(Process * process) {
	return 0;
}

std::vector<int> * MemStrategy::free(Process * process) {
	return NULL;
}

bool MemStrategy::alloc(Process * process) {
	return false;
}

std::string MemStrategy::getMemFrames(Process * process) {
	return "";
}

std::string MemStrategy::getName() {
	return "mem-strategy";
}
	
std::vector<std::vector<int>> * MemStrategy::evict(std::vector<Process*> queue, Process * process) {
	return NULL;
}
