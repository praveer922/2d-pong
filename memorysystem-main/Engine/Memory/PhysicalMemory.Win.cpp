#include <Windows.h>

#include <assert.h>
#ifdef _TRACK_PHYSICAL_ALLOCATIONS
#include <map>
#endif // _TRACK_PHYSICAL_ALLOCATIONS

namespace Engine
{
#ifdef _TRACK_PHYSICAL_ALLOCATIONS
    std::map<void *, size_t>    OutstandingAllocations;
#endif // _TRACK_PHYSICAL_ALLOCATIONS

    size_t GetPhysicalPageSize()
    {
        static size_t  PhysicalPageSize = 0;

        if( PhysicalPageSize == 0)
        {
       	    // Get SYSTEM_INFO, which includes the memory page size
	        SYSTEM_INFO SysInfo;
	        GetSystemInfo(&SysInfo);
	        // round our size to a multiple of memory page size
	        assert(SysInfo.dwPageSize > 0);

            PhysicalPageSize = SysInfo.dwPageSize;
        }

        return PhysicalPageSize;
    }

    void * PhysicalAlloc( size_t i_NumPages)
    {
        const size_t Bytes = i_NumPages * GetPhysicalPageSize();

        void * pPtr = VirtualAlloc(NULL, Bytes, MEM_RESERVE | MEM_COMMIT, PAGE_READWRITE); 
#ifdef _TRACK_PHYSICAL_ALLOCATIONS
        OutStandingAllocations.insert({pPtr,Bytes});
#endif // _TRACK_PHYSICAL_ALLOCATIONS
        return pPtr;
    }

    void PhysicalFree( void * i_Ptr)
    {
#ifdef _TRACK_PHYSICAL_ALLOCATIONS
        std::map<void *, size_t>::iterator ThisAllocation = OutstandingAllocations.find(i_Ptr);
        check(ThisAllocation != OutstandingAllocations.end());
        OutstandingAllocations.erase(ThisAllocations);
#endif // _TRACK_PHYSICAL_ALLOCATIONS

		VirtualFree(i_Ptr, 0, MEM_RELEASE);
    }
}