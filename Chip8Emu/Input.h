#pragma once
#ifndef INPUT_H
#define INPUT_H

#include <stdint.h>

extern void FillInputBuffer(uint16_t Input);
extern uint8_t IsButtonDown(uint8_t Button);

#endif // !INPUT_H