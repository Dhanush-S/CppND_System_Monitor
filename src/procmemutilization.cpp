#include "procmemutilization.h"

ProcMemUtilization::ProcMemUtilization(long size) : vmsize_(size){}

float ProcMemUtilization::MemoryUtilization() const{
    return vmsize_;
}

long ProcMemUtilization::VMSize() const
{
    return vmsize_;
}



