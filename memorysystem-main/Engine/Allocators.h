#pragma once

void* operator new(size_t i_size);
void operator delete(void* i_ptr);

namespace Engine
{
	void EnteringMain();
	void LeavingMain();
}

