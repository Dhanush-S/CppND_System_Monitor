#ifndef FORMAT_H
#define FORMAT_H

#include <string>

namespace Format {
//Using this namespace as Util namespace
//Helper functions placed here

int NumberOfDigits(int num);
std::string PrettyFormat(int num);
std::string ElapsedTime(long times);  
int ConvertToMB(long num);
};                                    

#endif