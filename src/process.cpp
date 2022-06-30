#include <unistd.h>
#include <cctype>
#include <sstream>
#include <string>
#include <vector>

#include "process.h"

using std::string;
using std::to_string;
using std::vector;

Process::Process(int pid) : pid_(pid)
{
    user_ = LinuxParser::User(pid);
    command_ = LinuxParser::Command(pid);
    uptime_ = (LinuxParser::UpTime() - LinuxParser::UpTime(pid));
    CalculateCPUUtilization();
    procMemUtilization = new ProcMemUtilization(LinuxParser::Ram(pid_));
}

void Process::CalculateCPUUtilization()
{
    auto procAttr = LinuxParser::ProcessCpuUtilization(pid_);
    long long systemUpTime = LinuxParser::UpTime(); 
    double utime = 0;
    long stime = 0;
    long cutime = 0;
    long cstime = 0;
    long starttime = 0;
    float hertz = 0;
    try
    {
        utime = std::stod(procAttr.at(13));
        stime = std::stod(procAttr.at(14));
        cutime = std::stold(procAttr.at(15));
        cstime = std::stol(procAttr.at(16));
        starttime = std::stol(procAttr.at(21));
        hertz = (float)sysconf(_SC_CLK_TCK);
    }
    catch(const std::exception& e)
    {
        utime = 0;
        stime = 0;
        cutime = 0;
        cstime = 0;
        starttime = 0;
        hertz = (float)sysconf(_SC_CLK_TCK);
    }

    float totaltime = (float)(utime + stime + cutime + cstime);
    float seconds = (float)systemUpTime - ((float)starttime/hertz);
    
    //Ensure divide by zero doesn't occur. Core dump will be observed otherwise
    if(seconds > 0)
        cpuutilization_ = (totaltime/hertz)/seconds;
    else
        cpuutilization_ = 0;
}

int Process::Pid() { return pid_; }

float Process::CpuUtilization() { return cpuutilization_; }

string Process::Command() { return command_; }

string Process::Ram() { 

int raminMB = Format::ConvertToMB(procMemUtilization->VMSize());
return std::to_string(raminMB); }

string Process::User() { return user_; }

long long Process::UpTime() { return uptime_; }

bool Process::operator<(Process const& a) const { 
    //return this->cpuutilization_ > a.cpuutilization_ ? true : false;
    return this->procMemUtilization->VMSize() > a.procMemUtilization->VMSize() ? true : false;
 }