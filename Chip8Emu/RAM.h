#pragma once
#ifndef RAM_H
#define RAM_H

#include <stdint.h>

#define RAM_SIZE (4096)
#define STACK_SIZE (16)

//0x00-0x4E contains hexadecimal font data
#define VM_GENERAL_START (0x4F)
//0x4F-0x1FF is general VM memory
#define PROGRAM_START (0x200)

uint8_t* Heap = NULL;

// ---- RAM MANAGEMENT SUBROUTINES ---- //

extern void InitRAM();

extern uint8_t ReadByte();
extern uint16_t ReadShort();

extern void CleanRAM();

#endif