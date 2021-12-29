#include "RAM.h"
#include "Backend.h"

uint8_t* Heap = NULL;

uint16_t Stack[STACK_SIZE];

uint16_t ProgramCounter = 0;
uint8_t StackPointer = 0;

static const uint8_t HexFont[0x50] =
{
	0xF0, 0x90, 0x90, 0x90, 0xF0, //0
	0x20, 0x60, 0x20, 0x20, 0x70, //1
	0xF0, 0x10, 0xF0, 0x80, 0xF0, //2
	0xF0, 0x10, 0xF0, 0x10, 0xF0, //3
	0x90, 0x90, 0xF0, 0x10, 0x10, //4
	0xF0, 0x80, 0xF0, 0x10, 0xF0, //5
	0xF0, 0x80, 0xF0, 0x90, 0xF0, //6
	0xF0, 0x10, 0x20, 0x40, 0x40, //7
	0xF0, 0x90, 0xF0, 0x90, 0xF0, //8
	0xF0, 0x90, 0xF0, 0x10, 0xF0, //9
	0xF0, 0x90, 0xF0, 0x90, 0x90, //A
	0xE0, 0x90, 0xE0, 0x90, 0xE0, //B
	0xF0, 0x80, 0x80, 0x80, 0xF0, //C
	0xE0, 0x90, 0x90, 0x90, 0xE0, //D
	0xF0, 0x80, 0xF0, 0x80, 0xF0, //E
	0xF0, 0x80, 0xF0, 0x80, 0x80  //F 
};

void InitRAM()
{
	//allocate space for heap
	Heap = (uint8_t*)malloc(RAM_SIZE);

	if (Heap == NULL)
	{
		printf("Heap allocation failed\n");
		return;
	}

	//Clean heap
	memset(Heap, 0, RAM_SIZE);

	//Clean stack
	memset(&Stack, 0, 2 * STACK_SIZE);

	//write hexadecimal font to RAM
	memcpy(Heap, &HexFont, 0x50);
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
		return;
	}

	BackendPanic("Stack overflow!");
	return;
}

uint16_t PopStack()
{
	if (StackPointer > 0)
	{
		StackPointer--;
		return Stack[StackPointer];
	}

	BackendPanic("Stack underflow!");
	return 0;
}

uint8_t ReadByte(uint16_t Address)
{
	if (Address > RAM_SIZE)
	{
		BackendPanic("ReadByte OOB!");
		return 0;
	}
	
	return Heap[Address];
}

uint16_t ReadShort(uint16_t Address)
{
	if (Address > RAM_SIZE)
	{
		BackendPanic("ReadShort OOB!");
		return 0;
	}

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