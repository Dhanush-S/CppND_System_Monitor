#ifndef SYSTEM_H
#define SYSTEM_H

#include <string>
#include <vector>

#include "process.h"
#include "processor.h"

#include "linux_parser.h"
#include "sysmemutilization.h"

class System {
 public:
  Processor& Cpu();                   
  std::vector<Process>& Processes();  
  float MemoryUtilization();          
  long UpTime();                      
  int TotalProcesses();               
  int RunningProcesses();             
  std::string Kernel();               
  std::string OperatingSystem();      
  long TotalJiffies();
  long ActiveJiffies();
  long IdleJiffies();

  //Define any necessary private members
 private:
  Processor cpu_ = {};
  std::vector<Process> processes_ = {};

  //Create a pointer to system memory utilization class 
  SysMemUtilization *memutilization_;

  //Adding as member variables as they are part of system class
  long totaljiffies_{0}, activejiffies_{0}, idlejiffies{0}; 
};

#endif