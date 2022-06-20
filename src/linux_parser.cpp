#include <dirent.h>
#include <unistd.h>
#include <sstream>
#include <string>
#include <vector>

#include "linux_parser.h"
#include "format.h"

using std::stof;
using std::string;
using std::to_string;
using std::vector;

//An example of how to read data from the filesystem
string LinuxParser::OperatingSystem() {
  string line;
  string key;
  string value;
  std::ifstream filestream(kOSPath);
  if (filestream.is_open()) {
    while (std::getline(filestream, line)) {
      std::replace(line.begin(), line.end(), ' ', '_');
      std::replace(line.begin(), line.end(), '=', ' ');
      std::replace(line.begin(), line.end(), '"', ' ');
      std::istringstream linestream(line);
      while (linestream >> key >> value) {
        if (key == "PRETTY_NAME") {
          std::replace(value.begin(), value.end(), '_', ' ');
          return value;
        }
      }
    }
  }
  return value;
}

//An example of how to read data from the filesystem
string LinuxParser::Kernel() {
  string os, kernel, version;
  string line;
  std::ifstream stream(kProcDirectory + kVersionFilename);
  if (stream.is_open()) {
    std::getline(stream, line);
    std::istringstream linestream(line);
    linestream >> os >> version >> kernel;
  }
  return kernel;
}

// BONUS: Update this to use std::filesystem
vector<int> LinuxParser::Pids() {
  vector<int> pids;
  DIR* directory = opendir(kProcDirectory.c_str());
  struct dirent* file;
  while ((file = readdir(directory)) != nullptr) {
    // Is this a directory?
    if (file->d_type == DT_DIR) {
      // Is every character of the name a digit?
      string filename(file->d_name);
      if (std::all_of(filename.begin(), filename.end(), isdigit)) {
        int pid = stoi(filename);
        pids.push_back(pid);
      }
    }
  }
  closedir(directory);
  return pids;
}

//Read and return the system memory utilization
SysMemUtilization LinuxParser::MemoryUtilization() { 
  
  long memtotal, memfree, memavailable, buffers, cached, sreclaimable, shmem;
  string line;
  string memoryinfo, memory, memoryunit;
  std::ifstream filestream(kProcDirectory + kMeminfoFilename);
  if (filestream.is_open()) {
    while (std::getline(filestream, line)) {
      std::replace(line.begin(), line.end(), ':', ' ');
      std::istringstream linestream(line);
      while (linestream >> memoryinfo >> memory >> memoryunit) {
        if (memoryinfo == "MemTotal") memtotal = std::stol(memory); 
        if (memoryinfo == "MemFree") memfree = std::stol(memory); 
        if (memoryinfo == "MemAvailable") memavailable = std::stol(memory); 
        if (memoryinfo == "Buffers") buffers = std::stol(memory); 
        if (memoryinfo == "Cached") cached = std::stol(memory); 
        if (memoryinfo == "SReclaimable") sreclaimable = std::stol(memory); 
        if (memoryinfo == "Shmem") shmem = std::stol(memory);      
      }
    }
  }
    
  return SysMemUtilization (memtotal,memfree,memavailable,buffers,cached,sreclaimable,shmem); 
  }

//Read and return the system uptime
long LinuxParser::UpTime() { 
  string uptime, idleprocesstime;
  string line;
  std::ifstream stream(kProcDirectory + kUptimeFilename);
  if (stream.is_open()) {
    std::getline(stream, line);
    std::istringstream linestream(line);
    linestream >> uptime >> idleprocesstime;
  }
  return std::stol(uptime);
}

//Read and return the number of jiffies for the system
long LinuxParser::Jiffies() { 
  return LinuxParser::UpTime() / sysconf(_SC_CLK_TCK);
}

//Read and return the number of active jiffies for a PID
long LinuxParser::ActiveJiffies(int pid) { 
  auto processAttributes = LinuxParser::ProcessCpuUtilization(pid);
  long activejiffies = 0;
  try
  {
    activejiffies = std::stol(processAttributes.at(13))+std::stol(processAttributes.at(14))+std::stol(processAttributes.at(15))+std::stol(processAttributes.at(16));  
  }
  catch(const std::exception& e)
  {
    // std::cerr << e.what() << '\n';
    return 0;
  }
    
  return activejiffies; 
}

//Read and return the number of active jiffies for the system
long LinuxParser::ActiveJiffies() { 
  auto cpuattributes = LinuxParser::CpuUtilization();

  long activejiffies = 0;
  for(auto cpuattribute : cpuattributes)
    activejiffies += std::stol(cpuattribute);

  return activejiffies; 
}

//Read and return the number of idle jiffies for the system
long LinuxParser::IdleJiffies() { 
  auto cpuattributes = LinuxParser::CpuUtilization();
  long idleJiffies = 0;

  //Catch vector related exceptions
  try
  {
    idleJiffies = std::stol(cpuattributes.at(3)) + std::stol(cpuattributes.at(4)); 
  }
  catch(const std::exception& e)
  {
    // std::cerr << e.what() << '\n';
    return 0;
  }
   
  return idleJiffies;
}

//Read and return CPU utilization
vector<string> LinuxParser::CpuUtilization() { 
  string line;
  string cpuutilparam;
  vector<string> cpuutilparams;
  std::ifstream filestream(kProcDirectory + kStatFilename);
  if (filestream.is_open()) {
    // Get only the first line from stat
    if (std::getline(filestream, line)) {
      std::istringstream linestream(line);
      while (linestream >> cpuutilparam)
      {
        if(cpuutilparam != "cpu")
            cpuutilparams.emplace_back(cpuutilparam);
      }    
    }
  }

 return cpuutilparams; 
}

//Read and return the total number of processes
int LinuxParser::TotalProcesses() { 
  string line;
  string key, numprocesses;
  std::ifstream filestream(kProcDirectory + kStatFilename);
  if (filestream.is_open()) {
    while (std::getline(filestream, line)) {
      std::istringstream linestream(line);
      while (linestream >> key >> numprocesses) {
        if (key == "processes") return std::stoi(numprocesses);
      }
    }
  }
  //Return 0 incase number of processes is not available
  return 0; 
  }

//Read and return the number of running processes
int LinuxParser::RunningProcesses() { 
  string line;
  string key, numprocs;
  std::ifstream filestream(kProcDirectory + kStatFilename);
  if (filestream.is_open()) {
    while (std::getline(filestream, line)) {
      std::istringstream linestream(line);
      while (linestream >> key >> numprocs) {
        if (key == "procs_running") return std::stoi(numprocs);
      }
    }
  }
  //Return 0 incase number of procs is not available
  return 0;
 }

//Read and return the command associated with a process
string LinuxParser::Command(int pid) { 
  string line;//, command;
  std::ifstream filestream(kProcDirectory +std::to_string(pid)+ kCmdlineFilename);
  if (filestream.is_open()) {
    if (std::getline(filestream, line)) {
      // std::istringstream linestream(line);
      // linestream >> command;
      return line;
    }
  }
  return line;
}  
 
//Read and return the memory used by a process
string LinuxParser::Ram(int pid) { 
  string line;
  string key, ramsize;
  std::ifstream filestream(kProcDirectory +std::to_string(pid)+ kStatusFilename);
  if (filestream.is_open()) {
    while (std::getline(filestream, line)) {
      std::replace(line.begin(), line.end(), ':', ' ');
      std::istringstream linestream(line);
      while (linestream >> key >> ramsize) {
        if (key == "VmSize") 
          return ramsize;
      }
    }
  }
  //Return empty string in case VmSize is not found
  return string(); 
}

//Read and return the user ID associated with a process
string LinuxParser::Uid(int pid) { 
  string line;
  string key, uid;
  std::ifstream filestream(kProcDirectory +std::to_string(pid)+ kStatusFilename);
  if (filestream.is_open()) {
    while (std::getline(filestream, line)) {
      std::replace(line.begin(), line.end(), ':', ' ');
      std::istringstream linestream(line);
      while (linestream >> key >> uid) {
        if (key == "Uid") return uid;
      }
    }
  }
  //return empty string if Uid is not found
  return string();  
}

//Read and return the user associated with a process
string LinuxParser::User(int pid) { 
  string uid = LinuxParser::Uid(pid);

  string line;
  string user, character, puserid;
  std::ifstream filestream(kPasswordPath);
  if (filestream.is_open()) {
    while (std::getline(filestream, line)) {
      std::replace(line.begin(), line.end(), ':', ' ');
      std::istringstream linestream(line);
      while (linestream >> user >> character >> puserid) {
        if (puserid == uid) return user;
      }
    }
  }
  //return empty string if user is not found
  return string(); 
}

//Read and return the uptime of a process
long LinuxParser::UpTime(int pid) { 
  auto processAttributes = LinuxParser::ProcessCpuUtilization(pid);
  long uptime = 0;
  try
  {
    uptime = std::stol(processAttributes.at(21));
  }
  catch(const std::exception& e)
  {
    // std::cerr << e.what() << '\n';
    return 0;
  }
    
  //Return seconds
  return (uptime/sysconf(_SC_CLK_TCK)); 
}

// Read and return cpu stat attributes for a particular process
vector<string> LinuxParser::ProcessCpuUtilization(int pid) { 
  string line;
  string pcpuutilparam;
  vector<string> pcpuutilparams;
  std::ifstream filestream(kProcDirectory + std::to_string(pid) + kStatFilename);
  if (filestream.is_open()) {
    // Read the whole file and extract each attribute
    while (std::getline(filestream, line)) {
      std::istringstream linestream(line);
      while (linestream >> pcpuutilparam)
        pcpuutilparams.emplace_back(pcpuutilparam);   
    }
  }
 return pcpuutilparams; 
}