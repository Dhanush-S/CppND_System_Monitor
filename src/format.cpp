#include <string>

#include "format.h"

using std::string;

//Time is a non-negative number and in decimal format. 
//Therefore no special handling of negative values required 
int Format::NumberOfDigits(int num)
{
  return std::to_string(num).length();
}

//Function to add additional '0' in case hour or min or sec is a single digit
std::string Format::PrettyFormat(int num)
{
  return NumberOfDigits(num) > 1 ? std::to_string(num) : ("0"+std::to_string(num));
}

//Extract different time parameters and return the string in HH:MM:SS format
string Format::ElapsedTime(long seconds) { 
  return PrettyFormat(seconds/3600)+":"+PrettyFormat((seconds/60)%60)+":"+PrettyFormat(seconds%60);  
}

//Function to convert any given KB to MB
int Format::ConvertToMB(long num)
{
  return (num/1024);
}
