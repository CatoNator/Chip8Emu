#include "Display.h"

void ClearScreen()
{
	for (int i = 0; i < RES_V; i++)
	{
		DisplayBuffer[i] = 0;
	}
}

void DrawPixel(uint8_t X, uint8_t Y)
{
	DisplayBuffer[Y] ^= (ZERO_PIX >> X);
}

void DrawSprite(uint8_t X, uint8_t Y, uint8_t* SprBuf, uint8_t Size)
{
	for (int i = 0; i < Size; i++)
	{
		DisplayBuffer[Y + i] ^= (SprBuf[i] >> X);
	}
}