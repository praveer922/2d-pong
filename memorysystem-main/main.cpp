#include "Engine/Allocators.h"

int main()
{
	Engine::EnteringMain();
	
	{
		int* ptr = new int;
	}

	Engine::LeavingMain();
	return 0;
}