#include "Helpers.h"
#include "PhysicalMemory.h"

#include <assert.h>
#include <intrin.h>
#include <memory.h>

namespace Engine
{
	inline void BitArray::Clear(void)
	{
		memset(m_arrayMemory, 0, m_arrayElements * sizeof(bitarray_t));
	}
	
	inline bool BitArray::IsEmpty(void) const
	{
		size_t i_dummyBit;

		return GetFirstSetBit(i_dummyBit) == false;
	}

	inline bool BitArray::IsFull(void) const
	{
		size_t i_dummyBit;

		return GetFirstClearBit(i_dummyBit) == false;
	}

	inline bool BitArray::IsBitSet(size_t i_bitNumber) const
	{
		assert(i_bitNumber < m_numBits);

		size_t element = i_bitNumber / s_bitsPerElement;
		size_t bit = i_bitNumber % s_bitsPerElement;

		assert(element < m_arrayElements);
		return (m_arrayMemory[element] & (1 << bit)) != 0;
	}

	inline bool BitArray::IsBitClear(size_t i_bitNumber) const
	{
		return !IsBitSet(i_bitNumber);
	}

	inline void BitArray::SetBit(size_t i_bitNumber)
	{
		assert(i_bitNumber < m_numBits);

		size_t element = i_bitNumber / s_bitsPerElement;
		bitarray_t bit = i_bitNumber % s_bitsPerElement;

		assert(element < m_arrayElements);
		m_arrayMemory[element] |= (1 << bit);
	}

	inline void BitArray::ClearBit(size_t i_bitNumber)
	{
		assert(i_bitNumber < m_numBits);

		size_t element = i_bitNumber / s_bitsPerElement;
		bitarray_t bit = i_bitNumber % s_bitsPerElement;

		assert(element < m_arrayElements);
		m_arrayMemory[element] &= ~(1 << bit);
	}

	inline bool BitArray::GetFirstClearBit(size_t& o_bitNumber) const
	{
		static const bitarray_t allSet = ~bitarray_t(0);

		size_t element = 0;

		while ((m_arrayMemory[element] == allSet) && (element < m_arrayElements))
			element++;

		if (element == m_arrayElements)
			return false;

		unsigned int bit = 0;
		bitarray_t bit_mask = 1;

		while ((m_arrayMemory[element] & bit_mask) != 0)
		{
			bit_mask <<= 1;
			bit++;
		}

		o_bitNumber = (element * s_bitsPerElement) + bit;
		return true;
	}

} // Namespace Engine
