#ifndef PROCESSOR_H
#define PROCESSOR_H

#include "linux_parser.h"

class Processor {
 public:
  float Utilization();  

  //Declare any necessary private members
 private:
    //CPU usage attributes
    //Can initialize a vector to store the values but the testing showed that vector operations take time resulting in potential 0 as CPU usage
    long user_{0}, nice_{0}, system_{0}, idle_{0}, iowait_{0}, irq_{0}, softirq_{0}, steal_{0}, guest_{0}, guest_nice_{0};
    
    //CPU attributes for previous second. 
    //Attributes correspond to previous second becuase the display update call is run for every 1 sec
    long prevtotal_{0}, previdle_{0};
};

#endif