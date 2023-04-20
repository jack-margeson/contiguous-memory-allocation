#include <vector>
#include <iostream>
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

    // Define parts of the command.
    unsigned char process = (unsigned char)((int)argv[1].at(1) + 1);
    long int size = stoi(argv[2]);
    string allocateMode = argv[3];
    long int freeMemory = getFreeMemory();

    // Check if a process with the same ID is currently assigned.
    for (vector<int> block : getContiguousMemory())
    {
        if (memory.at(block.at(0)) == process)
        {
            string s = "There is already memory allocated to a process with ID P";
            s.append(1, (unsigned char)((int)process - 1));
            s += ". Please specify a new process ID.";
            throw invalid_argument(s);
            return -1;
        }
    }
    // Check if we have enough free memory for the request.
    if (size <= freeMemory)
    {
        // Switch functionality depending on mode specified.
        if (allocateMode == "F")
        {
            int first = findFirst(size);
            if (first != -1)
            {
                // Allocate to first.
                for (int i = first; i < (first + size); i++)
                {
                    memory.at(i) = process;
                }
            }
            else
            {
                throw invalid_argument("There is not a contiguous space in memory free for the allocation. Please compact and try again.");
                return -1;
            }
        }
        else if (allocateMode == "B")
        {
            // Allocate to best.
        }
        else if (allocateMode == "W")
        {
            // Allocate to worst.
        }
        else
        {
            // Invalid allocation mode.
            throw invalid_argument("Invalid allocation mode.");
            return -1;
        }
    }
    else
    {
        throw invalid_argument("Not enough free memory to allocate " + to_string(size) + " bytes (free memory: " + to_string(freeMemory) + " bytes).");
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

    // Define parts of the command.
    unsigned char process = (unsigned char)((int)argv[1].at(1) + 1);

    // For each block of memory, if the process number matches, delete that block of memory.
    for (vector<int> block : getContiguousMemory())
    {
        if (memory.at(block.at(0)) == process)
        {
            // Delete starting from start of block to end of block.
            for (int i = block.at(0); i <= block.at(1); i++)
            {
                memory.at(i) = 0U;
            }
            return 0;
        }
    }

    string s = "Memory with process ID P";
    s.append(1, (unsigned char)((int)process - 1));
    s += " was not found.";
    throw invalid_argument(s);
    return -1;
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

    // Get the total size of unallocated memory.
    long int free = getFreeMemory();

    // Move all blocks of memory that are not free to the front of the memory space.
    long int front = 0;
    for (vector<int> block : getContiguousMemory())
    {
        if (memory.at(block.at(0)) != 0)
        {
            // Get size of current block of memory.
            long int size = block.at(1) - block.at(0) + 1;
            // Move to front of memory space.
            for (int i = front; i < front + size; i++)
            {
                memory.at(i) = memory.at(block.at(0));
            }
            front += size;
        }
    }
    // Set the last part of memory to the total unallocated size.
    for (int i = front; i < free; i++)
    {
        memory.at(i) = 0U;
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
    s += getContiguousMemoryString();
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
        if (argv.size() != 4 || argv.at(1).size() != 2 || argv.at(3).size() != 1)
        {
            throw invalid_argument("Invalid number or format of arguments for process memory allocation.");
        }
    }
    else if (argv[0] == "RL")
    {
        if (argv.size() != 2 || argv.at(1).size() != 2)
        {
            throw invalid_argument("Invalid number of arguments or format for process release.");
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

vector<vector<int>> Allocator::getContiguousMemory()
{
    vector<vector<int>> memoryBlocks;

    long int start;
    long int end;
    for (int i = 0; i < memory.size(); i++)
    {
        if (i != 0)
        {
            if (memory.at(i) != memory.at(i - 1))
            {
                end = i - 1;
                vector<int> block;
                block.push_back(start);
                block.push_back(end);
                memoryBlocks.push_back(block);

                start = i;
            }
            else if (i == memory.size() - 1)
            {
                end = memory.size();
                vector<int> block;
                block.push_back(start);
                block.push_back(end);
                memoryBlocks.push_back(block);
            }
        }
        else
        {
            start = 0;
        }
    }

    return memoryBlocks;
}

string Allocator::getContiguousMemoryString()
{
    string s = "";

    for (vector<int> block : getContiguousMemory())
    {
        // This block is unallocated space.
        if (memory.at(block.at(0)) == 0)
        {
            s += "Addresses [" + to_string(block.at(0)) + ":" + to_string(block.at(1)) + "], unallocated\n";
        }
        else
        { // Block space is allocated.
            s += "Addresses [" + to_string(block.at(0)) + ":" + to_string(block.at(1)) + "], process P" + char(int(memory.at(block.at(0))) - 1) + "\n";
        }
    }

    return s;
}

int Allocator::findFirst(long int size)
{
    for (vector<int> block : getContiguousMemory())
    {
        if (memory.at(block.at(0)) == 0 && block.at(1) - block.at(0) >= size)
        {
            return block.at(0);
        }
    }

    return -1;
}