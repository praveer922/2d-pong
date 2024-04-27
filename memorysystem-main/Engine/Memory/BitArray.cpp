#include "BitArray.h"

#include <assert.h>
#include <new>

namespace Engine
{
	BitArray* BitArray::Create(size_t i_numBits, bool i_startClear)
	{
		assert(i_numBits > 0);

		size_t arrayElements = (i_numBits + (s_bitsPerElement - 1)) / s_bitsPerElement;
		size_t arrayBytes = arrayElements * sizeof(bitarray_t);

		size_t pageSizeBytes = Engine::GetPhysicalPageSize();
		size_t arrayBytesInPages = AlignUp(arrayBytes + sizeof(BitArray), pageSizeBytes) / pageSizeBytes;
		
		void* pMemory = Engine::PhysicalAlloc(arrayBytesInPages);

		if (pMemory)
		{
			BitArray* pBitArray = reinterpret_cast<BitArray*>(pMemory);
			return new (pBitArray) BitArray(reinterpret_cast<bitarray_t*>(++pBitArray), arrayElements, i_numBits, i_startClear);
		}
		else
			return nullptr;
	}

	void BitArray::Destroy(BitArray* pBitArray)
	{
		if (pBitArray)
		{
			Engine::PhysicalFree(pBitArray);
		}
	}

	BitArray::BitArray(bitarray_t* i_arrayMemory, size_t i_arrayElements, size_t i_numBits, bool i_startClear) :
		m_arrayMemory(i_arrayMemory),
		m_arrayElements(i_arrayElements),
		m_numBits(i_numBits)
	{
		assert(i_arrayMemory);
		assert(i_arrayElements);
		assert(i_numBits);
		assert((i_arrayElements * s_bitsPerElement) >= i_numBits);

		int initValue = i_startClear ? 0 : ~0;

		memset(m_arrayMemory, initValue, i_arrayElements * sizeof(bitarray_t));
	}


	// specialization for unsigned long that uses an compiler intrinsic to find
	// the first bit set with 1 instruction
	bool BitArray::GetFirstSetBit(size_t& o_BitNumber) const
	{
		static const unsigned long allClear = bitarray_t(0);

		size_t element = 0;

		while ((m_arrayMemory[element] == allClear) && (element < m_arrayElements))
			element++;

		assert(element <= m_arrayElements);
		if (element == m_arrayElements)
			return false;

		unsigned long bit = 0;

#if _64BIT
		unsigned char bitFound = _BitScanForward64(&bit, m_arrayMemory[element]);
#else 
		unsigned char bitFound = _BitScanForward(&bit, m_arrayMemory[element]);
#endif // _64BIT

		assert(bitFound != 0);

		o_BitNumber = (element * s_bitsPerElement) + bit;

		return true;
	}


	void BitArray_UnitTest(void)
	{
		const unsigned int bitCount = 1000;

		BitArray* pMyArray = BitArray::Create(bitCount);

		pMyArray->SetBit(20);

		size_t firstSetBit = 0;
		bool foundSetBit = pMyArray->GetFirstSetBit(firstSetBit);
		assert(foundSetBit && (firstSetBit == 20));

		pMyArray->ClearBit(20);
		foundSetBit = pMyArray->GetFirstSetBit(firstSetBit);
		assert(foundSetBit == false);

		/*
			for( unsigned int i = 0; i < bitCount; i++ )
			{
				assert( pMyArray->IsBitClear( i ) == true );
				assert( pMyArray->IsBitSet( i ) == false );

				unsigned int bit = 0;

				pMyArray->GetFirstClearBit( bit );
				assert( bit == i );

				pMyArray->SetBit( i );

				assert( pMyArray->IsBitClear( i ) == false );
				assert( pMyArray->IsBitSet( i ) == true );

				pMyArray->GetFirstClearBit( bit );
				assert( bit == (i + 1) );
			}
		*/
		BitArray::Destroy(pMyArray);
	}

} // Namespace Engine