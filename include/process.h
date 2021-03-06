#ifndef PROCESS_H
#define PROCESS_H

#include <string>
#include "procmemutilization.h"
#include "format.h"

/*
Basic class for Process representation
It contains relevant attributes as shown below
*/
class Process {
 public:
  // void setPID(int pid){pid_ = pid;}
  Process(int pid);
  int Pid();                               // TODO: See src/process.cpp
  std::string User();                      // TODO: See src/process.cpp
  std::string Command();                   // TODO: See src/process.cpp
  float CpuUtilization();                  // TODO: See src/process.cpp
  std::string Ram();                       // TODO: See src/process.cpp
  long long UpTime();                       // TODO: See src/process.cpp
  bool operator<(Process const& a) const;  // TODO: See src/process.cpp

  // TODO: Declare any necessary private members
 private:
  int pid_{-1};
  std::string user_, command_;
  long long uptime_{0};
  float cpuutilization_{0.0};
  ProcMemUtilization* procMemUtilization;
  void CalculateCPUUtilization();
};

#endif