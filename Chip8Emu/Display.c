#include "Display.h"

uint8_t DisplayRefreshNeeded = 0;

void ClearScreen()
{
	for (int i = 0; i < RES_V; i++)
	{
		DisplayBuffer[i] = 0;
	}

	DisplayRefreshNeeded = 1;
}

void DrawPixel(uint8_t X, uint8_t Y)
{
	DisplayBuffer[Y] ^= (ZERO_PIX >> X);

	DisplayRefreshNeeded = 1;
}

void DrawSprite(uint8_t X, uint8_t Y, uint8_t* SprBuf, uint8_t Size)
{
	for (int i = 0; i < Size; i++)
	{
		uint64_t Line = (uint64_t)SprBuf[i];

		//I don't actually know why this is 53. By all accounts it should be 56 (64 - 8), but that doesn't work...
		Line <<= 53;
		DisplayBuffer[Y + i] ^= (Line >> X);
	}

	DisplayRefreshNeeded = 1;
}