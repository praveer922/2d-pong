#pragma once

namespace Engine
{
	class IAllocator
	{
	public:
		virtual void* _alloc(size_t i_size) = 0;
		virtual bool  _free(void* i_ptr) = 0;

		virtual bool Contains(void* i_ptr) const = 0;
	};
}
