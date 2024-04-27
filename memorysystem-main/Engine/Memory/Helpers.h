#pragma once

#include <assert.h>

template<typename T>
inline bool IsPowerOfTwo(T i_Value)
{
	return i_Value && !(i_Value & (i_Value - 1));;
}

template<typename T>
inline T AlignUp(T i_value, T i_align)
{
	assert(i_align);
	assert(IsPowerOfTwo(i_align));

	return (i_value + (i_align - 1)) & ~(i_align - 1);
}

template<typename T>
inline T AlignDown(T i_value, T i_align)
{
	assert(i_align);
	assert(IsPowerOfTwo(i_align));

	return i_value & ~(i_align - 1);
}

template<typename T>
inline T* AlignUp(void* i_addr, unsigned int i_align)
{
	assert(i_align);
	assert(IsPowerOfTwo(i_align));

	return reinterpret_cast<T*>((reinterpret_cast<uintptr_t>(i_addr) + (i_align - 1)) & ~uintptr_t(i_align - 1));
}

template<typename T>
inline T* AlignDown(void* i_addr, unsigned int i_align)
{
	assert(i_align);
	assert(IsPowerOfTwo(i_align));

	return reinterpret_cast<T*>(reinterpret_cast<uintptr_t>(i_addr) & ~uintptr_t((i_align - 1)));
}
