#include "Input.h"

uint16_t InputBuffer = 0;

void FillInputBuffer(uint16_t Input)
{
	InputBuffer = Input;
}

uint8_t IsButtonDown(uint8_t Button)
{
	Button &= 0x0F;

	uint16_t Mask = 1 << Button;

	return Mask & InputBuffer;
}