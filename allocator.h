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
        string getContiguousMemory();
};