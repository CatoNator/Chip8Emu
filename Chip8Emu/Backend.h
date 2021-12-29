#pragma once
#ifndef BACKEND_H
#define BACKEND_H

#include <stdint.h>
#include "Display.h"

extern uint8_t Running;

extern void BackendInit();
extern void BackendRun();
extern void BackendFinalDraw();
extern void BackendExit();

#endif