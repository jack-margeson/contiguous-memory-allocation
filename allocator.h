#include <string>
#include <vector>

using namespace std;

class Allocator
{
public:
    Allocator();
    Allocator(long int size);

    int requestMemory(string command);
    int releaseMemory(string command);
    int compactMemory(string command);
    string getMemoryStatus();

private:
    vector<unsigned char> memory;
    // Helper functions
    vector<string> parseCommand(string command);
    long int getFreeMemory();
    vector<vector<int>> getContiguousMemory();
    string getContiguousMemoryString();
    int findFirst(long int size);
    int findBest(long int size);
    int findWorst(long int size);
};