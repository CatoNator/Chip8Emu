#pragma once
#ifndef RAM_H
#define RAM_H

#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#define RAM_SIZE (4096)
#define STACK_SIZE (16)

//0x00-0x4E contains hexadecimal font data
#define VM_GENERAL_START (0x4F)
//0x4F-0x1FF is general VM memory
#define PROGRAM_START (0x200)

extern uint8_t* Heap;

extern uint16_t Stack[STACK_SIZE];

extern uint8_t ProgramCounter;
extern uint16_t StackPointer;

// ---- RAM MANAGEMENT SUBROUTINES ---- //

extern void InitRAM();

extern void PushStack(uint16_t Value);
extern uint16_t PopStack();

extern uint8_t ReadByte(uint16_t Address);
extern uint16_t ReadShort(uint16_t Address);

extern void CleanRAM();

#endif