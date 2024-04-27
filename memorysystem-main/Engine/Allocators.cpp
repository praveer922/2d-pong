#include "Memory\HeapManager.h"
#include "Memory\FixedSizeAllocator.h"

#include <assert.h>

#include <vector>
#include <utility>

namespace Engine
{

	static const std::pair<size_t, unsigned int> GlobalHeapInit = { 128 * 1024, 128 };
	static const std::pair<size_t, unsigned int> DefaultHeapInit = { 1024 * 1024, 1024 };

	static const std::pair<unsigned int, size_t> FixedSizeAllocatorsInit[] =
	{
		{ 16, 1000},
		{ 32, 1000},
		{ 128, 1000},
		{ 256, 1000}
	};

	Engine::HeapManager* GlobalsHeap = nullptr;
	Engine::HeapManager* DefaultHeap = nullptr;

	std::vector<std::pair<size_t, Engine::FixedSizeAllocator*> > FixedSizeAllocators;

	void EnteringMain()
	{
		using namespace Engine;

		assert(DefaultHeap == nullptr);

		DefaultHeap = HeapManager::Create(DefaultHeapInit.first, DefaultHeapInit.second);

		for (auto iter : FixedSizeAllocatorsInit)
		{
			FixedSizeAllocator* pAllocator = FixedSizeAllocator::Create(iter.first, iter.second);
			if (pAllocator)
			{
				FixedSizeAllocators.push_back({ iter.first,pAllocator });
			}
		}
	}

	void LeavingMain()
	{
		using namespace Engine;

		if (DefaultHeap)
		{
			HeapManager::Destroy(DefaultHeap);
			DefaultHeap = nullptr;
		}

		for (auto iter : FixedSizeAllocators)
		{
			if (iter.second)
			{
				FixedSizeAllocator::Destroy(iter.second);
			}
		}
		FixedSizeAllocators.clear();
	}

	Engine::IAllocator* GetAllocatorForSize(size_t i_size)
	{
		if (DefaultHeap == nullptr)
		{
			// we haven't entered main() yet so we use the GlobalsHeap, creating it if necessary
			if (GlobalsHeap == nullptr)
			{
				GlobalsHeap = Engine::HeapManager::Create(GlobalHeapInit.first, GlobalHeapInit.second);
			}

			return GlobalsHeap;
		}

		for (auto iter : FixedSizeAllocators)
		{
			if ((i_size <= iter.first) && iter.second)
			{
				return iter.second;
			}
		}

		return DefaultHeap;
	}

	Engine::IAllocator* GetAllocatorForPtr(void* i_ptr)
	{
		if (DefaultHeap && DefaultHeap->Contains(i_ptr))
		{
			return DefaultHeap;
		}

		for (auto iter : FixedSizeAllocators)
		{
			if (iter.second && iter.second->Contains(i_ptr))
			{
				return iter.second;
			}
		}

		return nullptr;
	}
} // namespace Engine

void* operator new(size_t i_size)
{
	using namespace Engine;

	IAllocator* pAllocator = GetAllocatorForSize(i_size);
	
	return pAllocator ? pAllocator->_alloc(i_size) : nullptr;
}

void operator delete(void* i_ptr)
{
	using namespace Engine;

	IAllocator* pAllocator = GetAllocatorForPtr(i_ptr);
	assert(pAllocator);

	pAllocator->_free(i_ptr);
}
