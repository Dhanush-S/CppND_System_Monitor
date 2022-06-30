#include "sysmemutilization.h"

  SysMemUtilization::SysMemUtilization(long memtotal, long memfree, long memavailable, long buffers, long cachemem, long sreclaimable, long shmem)
 {
    MemTotal(memtotal);
    MemFree(memfree);
    MemAvailable(memavailable);
    Buffers(buffers);
    CacheMem(cachemem);
    SReclaimable(sreclaimable);
    SHmem(shmem);
 }

 float SysMemUtilization::MemoryUtilization() const 
 {
    float totalmemused = memtotal_ - memfree_;
    float cachedmem = cachemem_ + sreclaimable_ - shmem_;
    //Calculate actual memory used
    float noncachemem = totalmemused - (buffers_ + cachedmem);

    //Return percentage of actual mem used against total mem available
    float percentage = 0;
    if(memtotal_ != 0)
      percentage = (noncachemem) / memtotal_;
      
    return percentage;
 }

 //Mutators
 void SysMemUtilization::MemTotal(long memtotal) { if(memtotal>0) memtotal_ = memtotal;}

 void SysMemUtilization::MemFree(long memfree) { if(memfree>0) memfree_ = memfree;}

 void SysMemUtilization::MemAvailable(long memavailable) { if(memavailable>0) memavailable_ = memavailable;}

 void SysMemUtilization::Buffers(long buffers) { if(buffers>0) buffers_ = buffers;}

 void SysMemUtilization::CacheMem(long cachemem) { if(cachemem>0) cachemem_ = cachemem;}

 void SysMemUtilization::SReclaimable(long sreclaimable) { if(sreclaimable>0) sreclaimable_ = sreclaimable;}

 void SysMemUtilization::SHmem(long shmem) { if(shmem>0) shmem_ = shmem;}

  //Accessors
 long SysMemUtilization::MemTotal() const { return memtotal_;}

 long SysMemUtilization::MemFree() const { return memfree_;}
 
 long SysMemUtilization::MemAvailable() const { return memavailable_;}

 long SysMemUtilization::Buffers() const { return buffers_;}

 long SysMemUtilization::CacheMem() const { return cachemem_;}

 long SysMemUtilization::SReclaimable() const { return sreclaimable_;}

 long SysMemUtilization::SHmem() const { return shmem_;}
