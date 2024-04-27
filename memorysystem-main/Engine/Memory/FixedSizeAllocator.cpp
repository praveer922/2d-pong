#include "FixedSizeAllocator.h"

#include <assert.h>
#include <new>

namespace Engine
{
	FixedSizeAllocator* FixedSizeAllocator::Create(unsigned int i_Size, size_t i_Num)
	{
		assert(i_Size > 0);
		assert(i_Num > 0);

		size_t BytesNeeded = i_Num * i_Size;

		size_t PhysicalPageBytes = Engine::GetPhysicalPageSize();
		size_t PhysicalPagesNeeded = AlignUp(BytesNeeded + sizeof(FixedSizeAllocator), PhysicalPageBytes) / PhysicalPageBytes;

		void* pMemory = PhysicalAlloc(PhysicalPagesNeeded);
		if (pMemory)
		{
			BitArray* pBitArray = BitArray::Create(i_Num, true);
			if (pBitArray)
			{
				FixedSizeAllocator* pAllocator = reinterpret_cast<FixedSizeAllocator*>(pMemory);
				return new (pAllocator) FixedSizeAllocator(i_Size, i_Num, ++pAllocator, pBitArray);
			}

			Engine::PhysicalFree(pMemory);
		}

		return nullptr;
	}

	void FixedSizeAllocator::Destroy(FixedSizeAllocator* i_pAllocator)
	{
		if (i_pAllocator)
		{
			assert(i_pAllocator->m_pInUseArray);
			assert(i_pAllocator->m_pInUseArray->IsEmpty());
			BitArray::Destroy(i_pAllocator->m_pInUseArray);

			Engine::PhysicalFree(i_pAllocator);
		}
	}

	FixedSizeAllocator::FixedSizeAllocator(unsigned int i_FixedSize, size_t i_FixedCount, void* i_pAllocatorMemory, BitArray* i_pInUseArray) :
		m_pAllocatorMemory(reinterpret_cast<uint8_t*>(i_pAllocatorMemory)),
		m_pInUseArray(i_pInUseArray),
		m_FixedSize(i_FixedSize),
		m_FixedCount(i_FixedCount)
	{
		assert(m_pAllocatorMemory);
		assert(m_pInUseArray);
	}

} // namespace Engine