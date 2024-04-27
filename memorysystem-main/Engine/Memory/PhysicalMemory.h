#pragma once

namespace Engine
{
    size_t GetPhysicalPageSize();
    void * PhysicalAlloc( size_t i_NumPages);
    void PhysicalFree( void * i_Ptr);
}