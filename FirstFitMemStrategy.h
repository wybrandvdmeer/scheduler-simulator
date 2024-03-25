#include <string>
#include <set>
#include <map>
#include <vector>

#include "MemStrategy.h"
#include "Process.h"

using namespace std;

class FirstFitMemStrategy : public MemStrategy {
public:
	int getMemUsage();
	bool alloc(Process * process);	
	int allocatedAt(Process *process);
	std::vector<int> * free(Process *process);
	std::string getMemFrames(Process * process);
	std::string getName();
	std::vector<std::vector<int>> * evict(std::vector<Process*> queue, Process * process);	

private:
	struct cmpStr {
		bool operator() (std::pair<int,int> const & lhs, std::pair<int,int> const & rhs) const {
    		return lhs.first < rhs.first;
  		}
	};
	std::map<Process *, std::pair<int,int>> allocs;
	std::pair<int,int> allocPair(int size, std::set<std::pair<int,int>, FirstFitMemStrategy::cmpStr> allocatedPairs);
	
};
