#ifndef PROC_MEM_UTILIZATION_H
#define PROC_MEM_UTILIZATION_H

#include "memutilization.h"
#include "linux_parser.h"

class ProcMemUtilization : MemUtilization
{
	public:
    ProcMemUtilization(long size);
    
    //Overriding pure virtual function of base class
    float MemoryUtilization() const override;

    //Mutators
    void VMSize(long memtotal);
    
    //Accessors : required to calculate different memory usage stats
    long VMSize() const;

    private:
    long vmsize_ = 0;

};
#endif