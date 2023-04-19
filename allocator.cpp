#include <vector>
#include <string>
#include <sstream>
#include <stdexcept>
#include "allocator.h"

Allocator::Allocator()
{
}

Allocator::Allocator(long int size)
{
    // Initalize allocator memory space to bytes of 0U.
    for (long int i = 0; i < size; i++)
    {
        memory.push_back(0U);
    }
}

int Allocator::requestMemory(string command)
{
    vector<string> argv;
    try
    {
        argv = parseCommand(command);
    }
    catch (invalid_argument &e)
    {
        throw e;
        return -1;
    }

    return 0;
}

int Allocator::releaseMemory(string command)
{
    vector<string> argv;
    try
    {
        argv = parseCommand(command);
    }
    catch (invalid_argument &e)
    {
        throw e;
        return -1;
    }

    return 0;
}

int Allocator::compactMemory(string command)
{
    // Check command validity.
    vector<string> argv;
    try
    {
        argv = parseCommand(command);
    }
    catch (invalid_argument &e)
    {
        throw e;
        return -1;
    }

    return 0;
}

string Allocator::getMemoryStatus()
{
    string s = "";
    s += "Total memory available: " + to_string(memory.size()) + " bytes\n";
    s += "Memory free for allocation: " + to_string(getFreeMemory()) + " bytes\n";
    s += "Current allocations by process ID:\n";
    s += "----------------------------------\n";
    s += getContiguousMemory();
    return s;
}

vector<string> Allocator::parseCommand(string command)
{
    vector<string> argv;
    stringstream ss(command);

    string s;
    while (ss >> s)
    {
        argv.push_back(s);
    }

    if (argv[0] == "RQ")
    {
        if (argv.size() != 4)
        {
            throw invalid_argument("Invalid number of arguments for process memory allocation.");
        }
    }
    else if (argv[0] == "RL")
    {
        if (argv.size() != 2)
        {
            throw invalid_argument("Invalid number of arguments for process release.");
        }
    }
    else if (argv[0] == "C")
    {
        if (argv.size() != 1)
        {
            throw invalid_argument("Invalid number of arguments for memory compaction.");
        }
    }

    return argv;
}

long int Allocator::getFreeMemory()
{
    long int free = 0;
    for (int i = 0; i < memory.size(); i++)
    {
        if (memory.at(i) == 0U)
        {
            free++;
        }
    }
    return free;
}

string Allocator::getContiguousMemory() {
    string s = "";

    long int start;
    long int end;
    
    for (int i = 0; i < memory.size(); i ++) {
        if (i != 0) {
            if (memory.at(i) != memory.at(i-1)) {
                end = i - 1;
                if (memory.at(i) == 0U) {
                    s += "Addresses [" + to_string(start) + ":" + to_string(end) + "], unallocated\n";
                } else {
                    s += "Addresses [" + to_string(start) + ":" + to_string(end) + "], process P" + to_string(int(memory.at(i))-1) + "\n";
                }
                start = i;
            }
        } else {
            start = 0;
        }
    }

    if (s == "") {
        s = "Addresses [0:" + to_string(memory.size()) + "], unallocated\n";
    }
    return s;
}