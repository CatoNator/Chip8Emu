#pragma once
#ifndef CPU_H
#define CPU_H

#include <stdint.h>

#define REGISTER_COUNT (16)
#define CPU_TICK_RATE (60)

//CPU registers, referred to as Vn in docs
extern uint8_t Registers[REGISTER_COUNT];

extern uint8_t DelayRegister;
extern uint8_t SoundTimerRegister;

extern uint16_t IndexRegister;

extern uint16_t InputBuffer;

extern void CPUInit();

extern void CPUTimerTick();

extern void ExecuteOpcode();

extern void OP_NOP(uint16_t OpCode);

#endif