#ifndef SYS_MEM_UTILIZATION_H
#define SYS_MEM_UTILIZATION_H

#include "memutilization.h"

class SysMemUtilization : MemUtilization
{
	public:
    //SysMemUtilization();
    SysMemUtilization(long memtotal, long memfree, long memavailable, long buffers, long cachemem, long sreclaimable, long shmem);

    //Overriding pure virtual function of base class
    float MemoryUtilization() const override;

    //Mutators
    void MemTotal(long memtotal);
    void MemFree(long memfree);
    void MemAvailable(long memavailable);
    void Buffers(long buffer);
    void CacheMem(long cachemem);
    void SReclaimable(long sreclaimable);
    void SHmem(long shmem);

    //Accessors : required to calculate different memory usage stats
    long MemTotal() const;
    long MemFree() const;
    long MemAvailable() const;
    long Buffers() const;
    long CacheMem() const;
    long SReclaimable() const;
    long SHmem() const;
    
    private:
    long memtotal_ = 0, memfree_ = 0, memavailable_ = 0, buffers_ = 0, cachemem_ = 0, sreclaimable_ = 0, shmem_ = 0;
    
};
#endif