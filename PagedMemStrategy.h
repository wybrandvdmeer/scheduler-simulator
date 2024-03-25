#include <string>
#include <set>
#include <map>

#include "MemStrategy.h"
#include "Process.h"

using namespace std;

class PagedMemStrategy : public MemStrategy {
public:
	int getMemUsage();
	bool alloc(Process * process);	
	int allocatedAt(Process *process);
	std::vector<int> * free(Process *process);
	std::string getMemFrames(Process * process);
	std::string getName();
	std::vector<std::vector<int>> * evict(std::vector<Process*> queue, Process * process);	
	vector<int> getAllocatedFrames();

private:
	std::vector<int> * allocate(int noOfPages, vector<int> v);
	std::map<Process *, std::vector<int> *> allocs;
};
