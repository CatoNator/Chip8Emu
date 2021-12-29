#include "RAM.h"

uint8_t* Heap = NULL;

uint16_t Stack[STACK_SIZE];

uint16_t ProgramCounter = 0;
uint16_t StackPointer = 0;

void InitRAM()
{
	//allocate space for heap
	Heap = (uint8_t*)malloc(RAM_SIZE);

	//Clean stack
	memset(&Stack, 0, 2 * STACK_SIZE);
}

void CleanRAM()
{
	free(Heap);
}

void PushStack(uint16_t Value)
{
	if (StackPointer < STACK_SIZE)
	{
		Stack[StackPointer] = Value;
		StackPointer++;
	}
}

uint16_t PopStack()
{
	if (StackPointer > 0)
	{
		StackPointer--;
		return Stack[StackPointer];
	}

	printf("Stack underflow!");
	return 0;
}

uint8_t ReadByte(uint16_t Address)
{
	return Heap[Address];
}

uint16_t ReadShort(uint16_t Address)
{
#ifdef SYS_LENDIAN
	//CHIP-8 is big-endian, therefore we need this little shuffle on little-endian machines

	uint16_t Value = 0;

	Value = Heap[Address] << 8;
	Value |= Heap[Address + 1];

	return Value;
#else
	return *(uint16_t*)&Heap[Address];
#endif
}