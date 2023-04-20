#include <iostream>
#include <string>
#include <stdexcept>
#include "allocator.h"

using namespace std;

// Function declarations
void terminal(Allocator &allocator);
void parseCommand(bool &active, Allocator &allocator, string command);

int main(int argc, char *argv[])
{
    // Create the allocator.
    Allocator allocator;
    try
    {
        allocator = Allocator(stoi(argv[1]));
    }
    catch (invalid_argument &e)
    {
        cerr << e.what() << "\n";
        return -1;
    }

    // Start terminal program.
    terminal(allocator);

    // End program.
    return 0;
}

void terminal(Allocator &allocator)
{
    bool active = true;

    // Print startup message.
    cout << "Contiguous Memory Allocation Simulation\n"
         << "---------------------------------\n";
    cout << "Enter 'HELP' for list of commands.\n\n";

    while (active)
    {
        string command = "";
        cout << "$ ";
        getline(cin, command);
        parseCommand(active, allocator, command);
        cout << "\n";
    }
}

void parseCommand(bool &active, Allocator &allocator, string command) {
if (command == "HELP")
    {
        cout << "\nList of commands:\n"
             << "HELP    - Displays this help message.\n"
             << "X, EXIT - Quits the program.\n"
             << "STAT    - Output current system information/status.\n"
             << "RQ      - Request memory.\n"
             << "\tex: RQ P0 2048 W, requests 2048 bytes from the\n"
             << "\tshared maximum memory for process ID 0, in W mode.\n"
             << "\tModes: F (first fit), B (best fit), W (worst fit)\n"
             << "RL      - Release memory by process ID.\n"
             << "\tex: RL P0, releases all memory from the\n"
             << "\trespective process ID.\n"
             << "C       - Compact unused holes in memory.\n";
    }
    else if (command == "EXIT" || command == "X" || command == "q")
    {
        cout << "Goodbye.\n";
        active = false;
    }
    else if (command == "STAT")
    {
        cout << "\n"
             << allocator.getMemoryStatus();
    }
    else if (command.rfind("RQ", 0) == 0)
    {
        try
        {
            int result = allocator.requestMemory(command);
        }
        catch (invalid_argument &e)
        {
            cout << e.what() << "\n";
        }
    }
    else if (command.rfind("RL", 0) == 0)
    {
        try
        {
            int result = allocator.releaseMemory(command);
        }
        catch (invalid_argument &e)
        {
            cout << e.what() << "\n";
        }
    }
    else
    {
        cout << "Invalid command. Please try again.\n";
    }
}