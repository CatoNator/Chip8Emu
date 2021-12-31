#pragma once
#ifndef DISPLAY_H
#define DISPLAY_H

#include <stdint.h>

#define RES_H (64)
#define RES_V (32)

#define ZERO_PIX (0x1000000000000000UL)

//Display buffer: 32 64-bit ints, for a 64x32 1bpp display

uint64_t DisplayBuffer[RES_V];

extern uint8_t DisplayRefreshNeeded;

extern void ClearScreen();
extern void DrawPixel(uint8_t X, uint8_t Y);
extern void DrawSprite(uint8_t X, uint8_t Y, uint8_t* SprBuf, uint8_t Size);

#endif