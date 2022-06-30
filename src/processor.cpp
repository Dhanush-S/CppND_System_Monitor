#include "processor.h"

//Return the aggregate CPU utilization
float Processor::Utilization() { 

//Fetch CPU utilization parameters
std::vector<std::string> cpuparameters = LinuxParser::CpuUtilization();

//Assign CPU parameters
/*
CPU attribute mapping
user       = 0
nice       = 1
system     = 2
idle       = 3
iowait     = 4
irq        = 5
softirq    = 6
steal      = 7
guest      = 8
guest_nice = 9
*/
try
{
    user_       = std::stol(cpuparameters[LinuxParser::kUser_]);
    nice_       = std::stol(cpuparameters[LinuxParser::kNice_]);
    system_     = std::stol(cpuparameters[LinuxParser::kSystem_]);
    idle_       = std::stol(cpuparameters[LinuxParser::kIdle_]);
    iowait_     = std::stol(cpuparameters[LinuxParser::kIOwait_]);
    irq_        = std::stol(cpuparameters[LinuxParser::kIRQ_]);
    softirq_    = std::stol(cpuparameters[LinuxParser::kSoftIRQ_]);
    steal_      = std::stol(cpuparameters[LinuxParser::kSteal_]);
    guest_      = std::stol(cpuparameters[LinuxParser::kGuest_]);
    guest_nice_ = std::stol(cpuparameters[LinuxParser::kGuestNice_]);
}
catch(const std::exception& e)
{
    //std::cerr << e.what() << '\n';
}

float currentIdle = idle_ + iowait_;
float currentNonIdle = user_ + nice_ + system_ + irq_ + softirq_ + steal_;
float currentTotal = currentIdle + currentNonIdle; 

//Differential values
float totaldiff = currentTotal - prevtotal_;
float idlediff  = currentIdle - previdle_;

float cpuusage = 0.0;
if(totaldiff != 0) cpuusage = (totaldiff - idlediff)/totaldiff;

//Save values for the next cycle
prevtotal_ = currentTotal;
previdle_  = currentIdle;

return cpuusage; 
}