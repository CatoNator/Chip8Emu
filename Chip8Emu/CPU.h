#pragma once
#ifndef CPU_H
#define CPU_H

#include <stdint.h>

#define REGISTER_COUNT (16)

//CPU registers, referred to as Vn in docs
uint8_t Registers[REGISTER_COUNT];

uint8_t DelayRegister = 0;
uint8_t SoundTimerRegister = 0;

extern void ExecuteOpcode();

// ---- OPCODES ---- //

extern void OP_SYS(); //jump to address N
extern void OP_CLS(); //Clear screen
extern void OP_RET(); //Return from subroutine
extern void OP_JP(); //Unconditional jump
extern void OP_CALL();
extern void OP_SE(); //Skip instruction if Vx is equal to value
extern void OP_SNE(); //Skip instruction if Vx is not equal to value
extern void OP_SE_REG(); //Skip instruction if Vx and Vy are equal
extern void OP_LD(); //Put value into register
extern void OP_ADD(); //Add value to register
extern void OP_LD_REG(); //Copy value from Vx to Vy
extern void OP_OR(); //Bitwise or on registers Vx and Vy, result stored in Vx
extern void OP_AND(); //Bitwise and on registers Vx and Vy, result stored in Vx
extern void OP_XOR(); //Same except bitwise xor
extern void OP_ADD_CAR(); //Same as ADD, but VF contains the carry bit
extern void OP_SUB(); //Subtracts Vy from Vx, stores result in Vx. VF contains carry bit if Vx > Vy
extern void OP_SHR(); //bitshift Vx right, VF stores bit in VF if LSB = 1
extern void OP_SUBN(); //Subtracts Vy from Vx, stores result in Vx. VF contains carry bit if Vy > Vx
extern void OP_SHL(); //Bitshift Vx left, stores bit in Vf if MSB = 1
extern void OP_SNE2(); //if values Vx and Vy are not equal, skips 2 instructions
extern void OP_LD_I(); //Register I is set to address n
extern void OP_JP_REG(); //Unconditional jump to V0 + n
extern void OP_RND(); //RNG generates a value from 0 to FF, which is bitwise and'd with value n. Result put in Vx.
extern void OP_DRW(); //Draw sprite from location I at (Vx, Vy), size n. VF sets collision
extern void OP_SKP(); //If key Vx is down, skip 2 instructions
extern void OP_SKNP(); //If key Vx is up, skip 2 instructions
extern void OP_LD_DT(); //Copy delay timer register value to Vx
extern void OP_LD_KP(); //Stall execution until a key is pressed, copy key value to Vx
extern void OP_LD_DT2(); //Set delay timer register to Vx
extern void OP_LD_ST(); //Set sound timer register to value Vx
extern void OP_ADD_I(); //Values from I and Vx are added, result stored in I
extern void OP_LD_F(); //I is set to the location of char Vx
extern void OP_LD_B(); //decimal representation of Vx is stored in I, I+1 and I+2 (little-endian)
extern void OP_LD_SV(); //Save registers V0 - Vx in memory at location I
extern void OP_LD_CP(); //Read values starting at I into registers V0 - Vx
//Super CHIP-48 instructions are unimplemented

#endif