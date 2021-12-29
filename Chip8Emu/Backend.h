#pragma once
#ifndef BACKEND_H
#define BACKEND_H

#include <stdint.h>
#include <stdio.h>
#include "CPU.h"
#include "Display.h"
#include "RAM.h"

extern uint8_t Running;

extern void BackendPanic(const char* Message);

extern void BackendInit();
extern uint8_t BackendLoadROM(const char* Path);
extern void BackendRun();
extern void BackendPollInput();
extern void BackendFinalDraw();
extern void BackendExit();

#endif