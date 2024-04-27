#include "HeapManager.h"
#include "Helpers.h"
#include "PhysicalMemory.h"

#include <assert.h>
#include <inttypes.h>
#include <memory.h>
#include <new>
#include <stdio.h>

// JLB-TODOx: test varying alignment
// JLB-TODOx: track allocations
// JLB-TODOx: verify descriptor count
// JLB-TODOx: implement destroy()
// JLB-TODOx: AllocFromBlockEnd() may use block descriptor - remove from m_pFreeList!!

namespace Engine
{
	const unsigned int	HeapManager::s_DefaultAlignment = sizeof(void*);
	const size_t		HeapManager::s_MinumumToLeave = 64;

#ifdef __GUARDBAND
	const uint8_t		HeapManager::guardbandValues[] = { 0xde, 0xad, 0xbe, 0xef }; // 0xdeadbeef
	const size_t		HeapManager::guardbandBytes = sizeof(guardbandValues);
#endif // __GUARDBAND

#ifdef __FILL
	const uint8_t		HeapManager::fillValueAlloc = 0xbb;
	const uint8_t		HeapManager::fillValueFree = 0xdd;
#endif // __FILL

	HeapManager* HeapManager::Create(size_t i_Bytes, unsigned int i_numDescriptors)
	{
		size_t PhysicalPageBytes = Engine::GetPhysicalPageSize();
		size_t PhysicalPagesNeeded = AlignUp(i_Bytes + sizeof(HeapManager), PhysicalPageBytes) / PhysicalPageBytes;

		void* pMemory = PhysicalAlloc(PhysicalPagesNeeded);
		if (pMemory)
		{
			HeapManager* pManager = reinterpret_cast<HeapManager*>(pMemory);
			return new (pManager) HeapManager(++pManager, i_Bytes, i_numDescriptors);
		}

		return nullptr;
	}

	void HeapManager::Destroy(HeapManager* i_pManager)
	{
		if (i_pManager)
		{
#ifdef __TRACK_ALLOCATIONS
			if (i_pManager->m_pAllocatedMemory)
				i_pManager->ShowOutstandingAllocations();
#endif // __TRACK_ALLOCATIONS

			unsigned int count = i_pManager->ListItems(i_pManager->m_pFreeMemory) + ListItems(i_pManager->m_pFreeDescriptors);
			assert(count == i_pManager->m_numDescriptors);
		}
	}

	HeapManager::HeapManager(void* i_pMemory, size_t i_sizeMemory, unsigned int i_numDescriptors) :
		m_sizeBase(i_sizeMemory),
#ifdef __TRACK_ALLOCATIONS
		m_pAllocatedMemory(nullptr),
#endif // __TRACK_ALLOCATIONS
		m_numDescriptors(i_numDescriptors)
	{
		assert(i_pMemory);
		assert(i_sizeMemory);
		assert(i_numDescriptors * sizeof(MemBlockDesc) < i_sizeMemory);

		m_pBase = reinterpret_cast<uint8_t*>(i_pMemory);

		m_pStartDescriptors = AlignUp<MemBlockDesc>(i_pMemory, __alignof(MemBlockDesc));

		InitializeFreeDescriptors(m_pStartDescriptors, m_numDescriptors);

		m_pAllocatable = AlignUp<uint8_t>(m_pStartDescriptors + m_numDescriptors, s_DefaultAlignment);
		m_sizeAllocatable = m_pBase + i_sizeMemory - m_pAllocatable;

		m_pFreeMemory = GetFreeDescriptor();
		assert(m_pFreeMemory);

		m_pFreeMemory->m_pBase = m_pAllocatable;
		m_pFreeMemory->m_size = m_sizeAllocatable;
		m_pFreeMemory->m_pNext = nullptr;
	}

	void* HeapManager::_alloc(size_t i_size, unsigned int i_alignment)
	{
		assert(i_size);
		assert(IsPowerOfTwo(i_alignment));

		MemBlockDesc* pCurrent = m_pFreeMemory;

		while (pCurrent)
		{
			void* pUserMem = AllocateFromBlockEnd(*pCurrent, i_size, i_alignment);
			if (pUserMem)
				return pUserMem;

			pCurrent = pCurrent->m_pNext;
		}

		return nullptr;
	}

	bool HeapManager::_free(void* i_ptr)
	{
		if (i_ptr == nullptr)
			return true;

		uint8_t* pAdjustedPtr = reinterpret_cast<uint8_t*>(i_ptr);

#ifdef __GUARDBAND
		pAdjustedPtr -= guardbandBytes;
#endif

		MemBlockDesc* pDesc = *AlignDown<MemBlockDesc*>(pAdjustedPtr - sizeof(MemBlockDesc*), __alignof(MemBlockDesc*));

		bool bIsValidDescriptor = IsValidDescriptorPtr(pDesc);
		assert(bIsValidDescriptor);

		if (!bIsValidDescriptor)
			return false;

#ifdef __GUARDBAND
		// Make sure the guardbands weren't trashed
		{
			uint8_t* pLowerGuardband = reinterpret_cast<uint8_t*>(i_ptr) - guardbandBytes;
			assert((pLowerGuardband[0] == guardbandValues[0]) && (pLowerGuardband[1] == guardbandValues[1])
				&& (pLowerGuardband[2] == guardbandValues[2]) && (pLowerGuardband[3] == guardbandValues[3]));

			uint8_t* pUpperGuardband = reinterpret_cast<uint8_t*>(i_ptr) + pDesc->m_sizeUserBlock;
			assert((pUpperGuardband[0] == guardbandValues[0]) && (pUpperGuardband[1] == guardbandValues[1])
				&& (pUpperGuardband[2] == guardbandValues[2]) && (pUpperGuardband[3] == guardbandValues[3]));
		}
		pDesc->m_sizeUserBlock = 0;
#endif // __GUARDBAND

#ifdef __FILL
		// fill the block with a fill value specific to free memory.
		memset(pDesc->m_pBase, fillValueFree, pDesc->m_size);
#endif // __FILL

#ifdef __TRACK_ALLOCATIONS
		// if we're tracking allocations pull it from the list of allocated blocks
		// and clean up our user block information
		{
			bool success = RemoveFromList(pDesc, m_pAllocatedMemory);
			assert(success);
		}
		pDesc->m_pUserBlock = NULL;
		pDesc->m_sizeUserBlock = 0;
#endif // __TRACK_ALLOCATIONS

		// Insert this block back into the free list in memory address sequential order
		InsertInListMemorySequentially(pDesc, m_pFreeMemory);

		return true;
	}

	void HeapManager::collect()
	{
		// Do garbage collection.
		// Since we're maintaining our free list in memory address sequential order
		// we only need to compare a block to the block after it to see if they adjoin.

		if (m_pFreeMemory)
		{
			MemBlockDesc* pCurrent = m_pFreeMemory;

			while (pCurrent->m_pNext)
			{
				MemBlockDesc* pNext = pCurrent->m_pNext;
				// Do this block and the block after it adjoin?
				if (pCurrent->m_pBase + pCurrent->m_size == pNext->m_pBase)
				{
					// If so consume it into our block, remove it from our list
					// and return it's descriptor to the free descriptor list
					pCurrent->m_size += pNext->m_size;
					pCurrent->m_pNext = pNext->m_pNext;

					ReturnFreeDescriptor(pNext);
				}
				else
					pCurrent = pCurrent->m_pNext;

			}
		}
	}

	void* HeapManager::AllocateFromBlockEnd(MemBlockDesc& i_Block, size_t i_size, unsigned int i_alignment)
	{
		size_t offsetFromEnd = i_size;

#ifdef __GUARDBAND
		// account for end guardband
		offsetFromEnd += guardbandBytes;
#endif

		// This is where the users memory starts. 
		uint8_t* pUserMemory = AlignDown<uint8_t>(reinterpret_cast<void*>(i_Block.m_pBase + i_Block.m_size - offsetFromEnd), i_alignment);

		// Now calculate where the block needs to start. We need some space
		// for the block start guardband and a cached pointer to the MemBlockDesc *
		uint8_t* pBlockStart = pUserMemory;
#ifdef __GUARDBAND
		// account for start guardband
		pBlockStart -= guardbandBytes;
#endif

		pBlockStart = AlignDown<uint8_t>(pBlockStart - sizeof(MemBlockDesc*), __alignof(MemBlockDesc*));

		// If this block isn't big enough we can't use it.
		if (pBlockStart < i_Block.m_pBase)
			return nullptr;

		// Decide whether to use this block wholly (if we just barely fit in it)
		// or whether to cut a chunk out and make a new block
		MemBlockDesc* pDesc = nullptr;

		// Use this block as is?
		if ((pBlockStart - i_Block.m_pBase) < s_MinumumToLeave)
		{
			// If we're going to use this block as is take it out of the free memory list
			RemoveFromList(&i_Block, m_pFreeMemory);
			pDesc = &i_Block;
		}
		else
		{
			// If not create a new tracked block of memory for this allocation
			pDesc = GetFreeDescriptor();
			assert(pDesc && "Out of descriptors.");

			if (!pDesc)
				return nullptr;

			pDesc->m_size = i_Block.m_pBase + i_Block.m_size - pBlockStart;
			pDesc->m_pBase = pBlockStart;
			pDesc->m_pNext = nullptr;

			i_Block.m_size = pBlockStart - i_Block.m_pBase;
		}

		assert(pDesc);

		// Cache a pointer to our MemBlockDesc for quick access on free
		MemBlockDesc** pCachedPtr = reinterpret_cast<MemBlockDesc**>(pBlockStart);
		*pCachedPtr = pDesc;

#ifdef __GUARDBAND
		// Record the user size so we know where the end guardband is
		pDesc->m_sizeUserBlock = i_size;

		// write the start of block guardband
		uint8_t* pStartGuardband = pUserMemory - guardbandBytes;
		pStartGuardband[0] = guardbandValues[0];
		pStartGuardband[1] = guardbandValues[1];
		pStartGuardband[2] = guardbandValues[2];
		pStartGuardband[3] = guardbandValues[3];

		// write the end of block guardband
		uint8_t* pEndGuardband = pUserMemory + i_size;
		pEndGuardband[0] = guardbandValues[0];
		pEndGuardband[1] = guardbandValues[1];
		pEndGuardband[2] = guardbandValues[2];
		pEndGuardband[3] = guardbandValues[3];
#endif // __GUARDBAND

#ifdef __FILL
		// fill the user memory area with a specific fill value
		memset(pUserMemory, fillValueAlloc, i_size);
#endif // __FILL

#ifdef __TRACK_ALLOCATIONS
		// If we're tracking outstanding allocations store the actual user
		// block start and size and add it this descriptor to the list of
		// allocated memory blocks.
		pDesc->m_sizeUserBlock = i_size;
		pDesc->m_pUserBlock = pUserMemory;
		InsertInListMemorySequentially(pDesc, m_pAllocatedMemory);
#endif // __TRACK_ALLOCATIONS

		return pUserMemory;
	}

	size_t HeapManager::getLargestFreeBlock() const
	{
		size_t largest = 0;

		MemBlockDesc* pCurrent = m_pFreeMemory;

		while (pCurrent)
		{
			if (pCurrent->m_size > largest)
				largest = pCurrent->m_size;

			pCurrent = pCurrent->m_pNext;
		}

		return largest;
	}

	size_t	HeapManager::getTotalFreeMemory() const
	{
		size_t total = 0;

		MemBlockDesc* pCurrent = m_pFreeMemory;

		while (pCurrent)
		{
			total += pCurrent->m_size;
			pCurrent = pCurrent->m_pNext;
		}

		return total;
	}

	unsigned int HeapManager::ListItems(const MemBlockDesc* i_pDesc)
	{
		unsigned int count = 0;

		while (i_pDesc)
		{
			count++;
			i_pDesc = i_pDesc->m_pNext;
		}

		return count;
	}

	void HeapManager::InsertInListMemorySequentially(MemBlockDesc* i_pDesc, MemBlockDesc*& io_pList)
	{
		MemBlockDesc** ppCurrent = &io_pList;

		while (*ppCurrent && (*ppCurrent)->m_pBase < i_pDesc->m_pBase)
			ppCurrent = &(*ppCurrent)->m_pNext;

		i_pDesc->m_pNext = *ppCurrent;
		*ppCurrent = i_pDesc;
	}

	bool HeapManager::RemoveFromList(MemBlockDesc* i_pDesc, MemBlockDesc*& io_pList)
	{
		MemBlockDesc** ppCurrent = &io_pList;

		while (*ppCurrent)
		{
			if (*ppCurrent == i_pDesc)
			{
				*ppCurrent = i_pDesc->m_pNext;

				i_pDesc->m_pNext = nullptr;

				return true;
			}

			ppCurrent = &(*ppCurrent)->m_pNext;
		}

		return false;
	}

	bool HeapManager::IsAllocated(void* i_ptr) const
	{
		uint8_t* pAdjustedPtr = reinterpret_cast<uint8_t*>(i_ptr);

#ifdef __GUARDBAND
		pAdjustedPtr -= guardbandBytes;
#endif

		MemBlockDesc* pDesc = *AlignDown<MemBlockDesc*>(pAdjustedPtr - sizeof(MemBlockDesc*), __alignof(MemBlockDesc*));

		return IsValidDescriptorPtr(pDesc);
	}

	void HeapManager::InitializeFreeDescriptors(MemBlockDesc* i_pFirstDescriptor, size_t i_numDescriptors)
	{
		m_pFreeDescriptors = i_pFirstDescriptor;

		for (size_t i = 0; i < (i_numDescriptors - 1); i++)
		{
			i_pFirstDescriptor->m_size = 0;
			i_pFirstDescriptor->m_pBase = nullptr;

			i_pFirstDescriptor->m_pNext = i_pFirstDescriptor + 1;

			i_pFirstDescriptor++;
		}

		i_pFirstDescriptor->m_pNext = nullptr;

#ifdef _DEBUG
		// Sanity check to make sure I linked them all together correctly.
		// Make sure the linked list count == the number we started with
		size_t count = 0;

		MemBlockDesc* pCurrent = m_pFreeDescriptors;

		while (pCurrent && (count < i_numDescriptors))
		{
			count++;
			pCurrent = pCurrent->m_pNext;
		}

		assert((count == i_numDescriptors) && (pCurrent == nullptr));
#endif // _DEBUG
	}

	HeapManager::MemBlockDesc* HeapManager::GetFreeDescriptor(void)
	{
		MemBlockDesc* pReturnPtr = m_pFreeDescriptors;

		if (m_pFreeDescriptors)
			m_pFreeDescriptors = pReturnPtr->m_pNext;

		return pReturnPtr;
	}

	void HeapManager::ReturnFreeDescriptor(MemBlockDesc* i_pDesc)
	{
#ifdef _DEBUG
		i_pDesc->m_pBase = nullptr;
		i_pDesc->m_size = 0;
#endif // _DEBUG

		i_pDesc->m_pNext = m_pFreeDescriptors;

		m_pFreeDescriptors = i_pDesc;
	}

	bool HeapManager::IsValidDescriptorPtr(const MemBlockDesc* i_pDesc) const
	{
		if (i_pDesc < m_pStartDescriptors)
			return false;
		if (i_pDesc >= (m_pStartDescriptors + m_numDescriptors))
			return false;

		if ((reinterpret_cast<uintptr_t>(i_pDesc) - reinterpret_cast<uintptr_t>(m_pStartDescriptors)) % sizeof(MemBlockDesc) != 0)
			return false;

		return true;
	}

#if _64BIT
#define POINTERCHARS 16
#else
#define POINTERCHARS 8
#endif // _64BIT

	// Handy C preprocessor macros to convert text to quoted strings
#define AS_STRING(str) #str
#define DEF_AS_STRING(def) AS_STRING(def)

	void HeapManager::ShowFreeBlocks() const
	{
		MemBlockDesc* pCurrent = m_pFreeMemory;
		printf("Free Blocks:\n");
		if (pCurrent)
		{
			while (pCurrent)
			{
				printf("\t Block:  0x0" DEF_AS_STRING(POINTERCHARS) "%" PRIxPTR " %8zu bytes\n", (uintptr_t)pCurrent->m_pBase, pCurrent->m_size);
				pCurrent = pCurrent->m_pNext;
			}
		}
		else
			printf("\tNone.\n");

	}

#ifdef __TRACK_ALLOCATIONS
	void HeapManager::ShowOutstandingAllocations() const
	{
		MemBlockDesc* pCurrent = m_pAllocatedMemory;
		printf("Outstanding allocations:\n");
		if (pCurrent)
		{
			while (pCurrent)
			{
				printf("\t Block:  0x0" DEF_AS_STRING(POINTERCHARS) "%" PRIxPTR" %8zu bytes  ", (uintptr_t)pCurrent->m_pBase, pCurrent->m_size);
				printf("User Block: 0x0" DEF_AS_STRING(POINTERCHARS) "%" PRIxPTR" %8zu bytes\n", (uintptr_t)pCurrent->m_pUserBlock, pCurrent->m_sizeUserBlock);
				pCurrent = pCurrent->m_pNext;
			}
		}
		else
			printf("\tNone.\n");
	}
#endif // __TRACK_ALLOCATIONS

} // namespace Engine
