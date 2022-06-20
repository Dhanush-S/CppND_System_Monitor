#ifndef MEM_UTILIZATION_H
#define MEM_UTILIZATION_H

//Base class for memory utilization of system and process
class MemUtilization
{
    virtual float MemoryUtilization() const = 0;
};

#endif