#include <assert.h>

namespace Engine
{

	inline void* HeapManager::_alloc(size_t i_size)
	{
		return _alloc(i_size, s_DefaultAlignment);
	}

	inline bool HeapManager::Contains(void* i_ptr) const
	{
		return (i_ptr >= m_pAllocatable) && (i_ptr < (m_pAllocatable + m_sizeBase));
	}
} // namespace Engine

