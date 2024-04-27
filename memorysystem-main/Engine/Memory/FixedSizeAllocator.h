#pragma once

#include "IAllocator.h"
#include "BitArray.h"
#include "Helpers.h"

namespace Engine
{

class FixedSizeAllocator : public IAllocator
{
public:
	static FixedSizeAllocator* Create(unsigned int i_Size, size_t i_Num);
	static void Destroy(FixedSizeAllocator* pAllocator);

	void * _alloc(size_t i_Size)
	{
		assert(i_Size == m_FixedSize);

		size_t BitArrayIndex = 0;
		if(m_pInUseArray->GetFirstClearBit(BitArrayIndex))
		{
			m_pInUseArray->SetBit(BitArrayIndex);
			return m_pAllocatorMemory + (BitArrayIndex * m_FixedSize);
		}
		else
			return nullptr;
	}

	bool _free(void* i_pPtr)
	{
		if (i_pPtr)
		{
			// ensure i_pPtr is within our bounds
			assert((i_pPtr >= m_pAllocatorMemory) && (i_pPtr < (m_pAllocatorMemory + (m_FixedSize * m_FixedCount))));
			// ensure i_pPtr is a valid offset with our memmory
			assert(((reinterpret_cast<uint8_t*>(i_pPtr) - m_pAllocatorMemory) % m_FixedSize) == 0);
			size_t BitArrayIndex = (reinterpret_cast<uint8_t*>(i_pPtr) - m_pAllocatorMemory) / m_FixedSize;
			// ensure that bit is set (means it's an outstanding allocation)
			assert(m_pInUseArray->IsBitSet(BitArrayIndex));
			
			// clear that bit (denotes that block is now available again
			m_pInUseArray->ClearBit(BitArrayIndex);
		}

		return true;
	}

	bool Contains(void* i_Ptr) const
	{
		return	(i_Ptr >= m_pAllocatorMemory) && (i_Ptr < (m_pAllocatorMemory + (m_FixedSize * m_FixedCount)));
	}

private:
	FixedSizeAllocator(unsigned int i_FixedSize, size_t i_FixedCount, void* i_pAllocatorMemory, BitArray* i_pInUseArray);
	~FixedSizeAllocator() = delete;

	//disable the copy constructor and assignment operator
	FixedSizeAllocator(const FixedSizeAllocator&) = delete;
	FixedSizeAllocator& operator=(const FixedSizeAllocator&) = delete;

	uint8_t* m_pAllocatorMemory;
	BitArray* m_pInUseArray;

	unsigned int m_FixedSize;
	size_t m_FixedCount;
};
} // namespace Engine
