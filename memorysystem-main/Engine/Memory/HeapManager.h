#pragma once
#include "IAllocator.h"
#include <stdint.h>

namespace Engine
{
	class HeapManager;

	class HeapManager : public IAllocator
	{
	public:
		static HeapManager* Create(size_t i_Bytes, unsigned int i_numDescriptors);
		static void Destroy(HeapManager* i_Manager);

		void*			_alloc(size_t i_size);
		void*			_alloc(size_t i_size, unsigned int i_alignment);
		bool			_free(void* i_ptr);

		void			collect();

		bool			Contains(void* i_ptr) const;
		bool			IsAllocated(void* i_ptr) const;

		size_t			getLargestFreeBlock() const;
		size_t			getTotalFreeMemory() const;

		void			ShowFreeBlocks() const;

#ifdef __TRACK_ALLOCATIONS
		void			ShowOutstandingAllocations() const;
#endif // __TRACK_ALLOCATIONS

	private:
		HeapManager(void* i_pMemory, size_t i_sizeMemory, unsigned int i_numDescriptors);
		~HeapManager() = delete;

		typedef struct MemBlockDescriptor
		{
			struct MemBlockDescriptor* m_pNext;
			uint8_t* m_pBase;
			size_t                      m_size;
#ifdef __TRACK_ALLOCATIONS
			uint8_t* m_pUserBlock;
#endif // __TRACK_ALLOCATIONS
#if defined(__GUARDBAND) || defined(__TRACK_ALLOCATIONS)
			size_t						m_sizeUserBlock;
#endif // __GUARDBAND || __TRACK_ALLOCATIONS
		} MemBlockDesc;

		void* AllocateFromBlockEnd(MemBlockDesc& i_Block, size_t i_size, unsigned int i_alignment);

		void			InitializeFreeDescriptors(MemBlockDesc* i_pFirstDescriptor, size_t i_numDescriptors);

		MemBlockDesc* GetFreeDescriptor();
		void			ReturnFreeDescriptor(MemBlockDesc* i_pDesc);
		bool			IsValidDescriptorPtr(const MemBlockDesc* i_pDesc) const;

		static unsigned int	ListItems(const MemBlockDesc* i_pDesc);
		static void		InsertInListMemorySequentially(MemBlockDesc* i_pDesc, MemBlockDesc*& io_pList);
		static bool		RemoveFromList(MemBlockDesc* i_pDesc, MemBlockDesc*& io_pList);

		// our list of blocks of free memory
		MemBlockDesc* m_pFreeMemory;

#ifdef __TRACK_ALLOCATIONS
		// our list of blocks of allocated memory
		MemBlockDesc* m_pAllocatedMemory;
#endif // __TRACK_ALLOCATIONS

		// our list of free block descriptors
		MemBlockDesc* m_pFreeDescriptors;

		// keep track of the entire memory range
		uint8_t* m_pBase;
		size_t					m_sizeBase;

		// keep track of the memory range used for descriptors
		MemBlockDesc* m_pStartDescriptors;
		unsigned int			m_numDescriptors;

		// keep track of the memory range that's allocatable
		uint8_t* m_pAllocatable;
		size_t					m_sizeAllocatable;

		static const unsigned int	s_DefaultAlignment;
		static const size_t			s_MinumumToLeave;

#ifdef __GUARDBAND
		static const uint8_t		guardbandValues[];
		static const size_t			guardbandBytes;
#endif // __GUARDBAND

#ifdef __FILL
		static const uint8_t		fillValueAlloc;
		static const uint8_t		fillValueFree;
#endif // __FILL
	};

} // namespace Engine

#include "HeapManager-inl.h"
