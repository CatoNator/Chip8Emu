#include "RAM.h"

void InitRAM()
{
	//allocate space for heap
	if (Heap == NULL)
	{
		Heap = (uint8_t*)malloc(RAM_SIZE);
	}
}

void CleanRAM()
{
	if (Heap != NULL)
	{
		free(Heap);
		Heap = NULL;
	}
}