#pragma once

#include <stdint.h>

namespace Engine
{
#if _64BIT
	typedef uint64_t	bitarray_t;
#else
	typedef uint32_t	bitarray_t;
#endif // _64BIT

	class BitArray
	{
	public:
		static BitArray* Create(size_t i_numBits, bool i_startClear = true);
		static void Destroy(BitArray* pBitArray);

		void Clear(void);

		bool IsEmpty(void) const;
		bool IsFull(void) const;

		inline bool IsBitSet(size_t i_bitNumber) const;
		inline bool IsBitClear(size_t i_bitNumber) const;

		void SetBit(size_t i_bitNumber);
		void ClearBit(size_t i_bitNumber);

		bool GetFirstClearBit(size_t& o_bitNumber) const;
		bool GetFirstSetBit(size_t& o_bitNumber) const;
	private:
		BitArray(bitarray_t* i_BitArrayMemory, size_t i_BitArrayElements, size_t i_NumBits, bool i_StartClear);
		~BitArray() = delete;

		// make non-copyable
		BitArray(const BitArray&) = delete;
		void operator=(const BitArray&) = delete;

		static const unsigned int s_bitsPerElement = sizeof(void*) * 8;

		bitarray_t* m_arrayMemory;
		size_t		m_arrayElements;
		size_t		m_numBits;
	};

	void BitArray_UnitTest(void);

} // Namespace Engine

#include "BitArray.inl"