#include <unistd.h>
#include <cstddef>
#include <set>
#include <string>
#include <vector>

#include "process.h"
#include "processor.h"
#include "system.h"

using std::set;
using std::size_t;
using std::string;
using std::vector;
/*You need to complete the mentioned TODOs in order to satisfy the rubric criteria "The student will be able to extract and display basic data about the system."

You need to properly format the uptime. Refer to the comments mentioned in format. cpp for formatting the uptime.*/

//Return the system's CPU
Processor& System::Cpu() { 
    return cpu_; }

//Return a container composed of the system's processes
vector<Process>& System::Processes() { 

    //Clean processes_ vector to ensure processes are note repeated
    processes_.clear();

    //Obtain PIDs    
    auto pids = LinuxParser::Pids();
    
    //For each pid, calculate process parameters and create process object
    for(auto pid : pids)
    {
        Process process(pid);
        processes_.emplace_back(process);
    }

    //Sort based on RAM usage
    std::sort(processes_.begin(),processes_.end());

    return processes_; 
}

// DONE: Return the system's kernel identifier (string)
std::string System::Kernel() { return LinuxParser::Kernel(); }

//Return the system's memory utilization
float System::MemoryUtilization() { 
    //storing system mem utilization object for future use as part of system object
    SysMemUtilization temp= LinuxParser::MemoryUtilization();

    //allocate memory only if the method MemoryUtilization is indeed called
    memutilization_ = new SysMemUtilization(temp.MemTotal(),temp.MemFree(),temp.MemAvailable(),temp.Buffers(),temp.CacheMem(),temp.SReclaimable(),temp.SHmem());
    
    return memutilization_->MemoryUtilization();
}

//Return the operating system name
std::string System::OperatingSystem() { return LinuxParser::OperatingSystem(); }

//Return the number of processes actively running on the system
int System::RunningProcesses() { return LinuxParser::RunningProcesses();  }

//Return the total number of processes on the system
int System::TotalProcesses() { return LinuxParser::TotalProcesses(); }

//Return the number of seconds since the system started running
long int System::UpTime() { return LinuxParser::UpTime(); }

long System::TotalJiffies() {return LinuxParser::Jiffies();}

long System::ActiveJiffies() {return LinuxParser::ActiveJiffies();}

long System::IdleJiffies() {return LinuxParser::IdleJiffies();}