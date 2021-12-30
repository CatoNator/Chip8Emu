#include "Input.h"

uint16_t InputBuffer = 0;

uint8_t IsButtonDown(uint8_t Button)
{
	Button &= 0x0F;

	return ((1 << Button) & InputBuffer) != 0;
}