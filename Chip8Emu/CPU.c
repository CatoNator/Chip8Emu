#include "CPU.h"
#include "RAM.h"
#include "Display.h"

uint8_t Registers[REGISTER_COUNT];

uint8_t DelayRegister = 0;
uint8_t SoundTimerRegister = 0;

uint16_t IRegister = 0;

extern void ExecuteOpcode()
{
	uint16_t OpCode = ReadShort(ProgramCounter);
	ProgramCounter += 2;

	uint8_t VX = (OpCode >> 8) & 0x000F; //Lower nibble of high byte
	uint8_t VY = (OpCode >> 8) & 0x000F; //Upper nibble of low byte
	uint8_t N = OpCode & 0x000F; //Lowest nibble
	uint8_t VAL = OpCode & 0x00FF; //Lower byte
	uint16_t ADDR = OpCode & 0x0FFF; //Address is lowest 12 bits

	switch (OpCode & 0xF000)
	{
	case (0x0000):
	{
		switch (OpCode)
		{
		case (0x00E0): //CLS
			ClearScreen();
			break;
		case (0x00EE): //RET
			ProgramCounter = PopStack();
			break;
		default: //SYS addr
			OP_NOP();
			break;
		}
		break;
	}
	case (0x1000):
	{
		//JP addr
		ProgramCounter = ADDR;
		break;
	}
	case (0x2000):
	{
		//CALL
		PushStack(ProgramCounter);
		ProgramCounter = ADDR;
		break;
	}
	case (0x3000):
	{
		//SE Vx, byte
		if (Registers[VX] == VAL)
			ProgramCounter += 2;

		break;
	}
	case (0x4000):
	{
		//SNE Vx, byte
		if (Registers[VX] != VAL)
			ProgramCounter += 2;

		break;
	}
	case (0x5000):
	{
		//SE Vx, Vy
		if (Registers[VX] == Registers[VY])
			ProgramCounter += 2;

		break;
	}
	case (0x6000):
	{
		//LD Vx, byte
		Registers[VX] = VAL;
		break;
	}
	case (0x7000):
	{
		//ADD Vx, byte
		Registers[VX] += VAL;

		break;
	}
	case (0x8000):
	{
		switch (OpCode & 0x000F)
		{
		case (0):
		{
			//LD Vx, Vy
			Registers[VX] = Registers[VY];

			break;
		}
		case (1):
		{
			//OR Vx, Vy
			Registers[VX] |= Registers[VY];

			break;
		}
		case (2):
		{
			//AND Vx, Vy
			Registers[VX] &= Registers[VY];

			break;
		}
		case (3):
		{
			//XOR Vx, Vy
			Registers[VX] ^= Registers[VY];

			break;
		}
		case (4):
		{
			//ADD Vx, Vy
			uint16_t Val = Registers[VX] + Registers[VY];

			//carry bit
			if (Val > 0xFF)
				Registers[0xF] = 1;

			Registers[VX] = (uint8_t)(Val & 0x00FF);

			break;
		}
		case (5):
		{
			//SUB Vx, Vy

			//weird bit thingy
			if (Registers[VX] > Registers[VY])
				Registers[0xF] = 1;

			Registers[VX] -= Registers[VY];

			break;
		}
		case (6):
		{
			//SHR Vx

			//LSB storage
			if (Registers[VX] & 0x01)
				Registers[0xF] = 1;

			Registers[VX] >>= 1;

			break;
		}
		case (7):
		{
			//SUBN Vx, Vy

			//weird bit thingy - inverse of 0x8xy5
			if (Registers[VY] > Registers[VX])
				Registers[0xF] = 1;

			Registers[VX] -= Registers[VY];

			break;
		}
		case (0xE):
		{
			//SHL Vx

			//MSB storage
			if (Registers[VX] & 0x80)
				Registers[0xF] = 1;

			Registers[VX] <<= 1;

			break;
		}
		}
		break;
	}
	case (0x9000):
	{
		//SNE Vx, Vy

		if (Registers[VX] != Registers[VY])
			ProgramCounter += 2;

		break;
	}
	case (0xA000):
	{
		//LD I, addr

		IRegister = ADDR;

		break;
	}
	case (0xB000):
	{
		//JP V0, addr

		ProgramCounter = Registers[0] + ADDR;

		break;
	}
	case (0xC000):
	{
		//DRW Vx, Vy, n

		DrawSprite(Registers[VX], Registers[VY], (uint8_t*)(Heap + IRegister), N);

		break;
	}
	case (0xE000):
	{
		switch (OpCode & 0x00FF)
		{
		case (0x9E):
		{
			//SKP Vx
			//to do

			/*
			if (IsKeyDown(Registers[VX])
				ProgramCounter += 2;
			*/

			break;
		}
		case (0xA1):
		{
			//SKNP Vx
			//to do

			/*
			if (!IsKeyDown(Registers[VX])
				ProgramCounter += 2;
			*/

			break;
		}
		default:
			OP_NOP();
			break;
		}
	}
	case (0xF000):
	{
		switch (OpCode & 0x00FF)
		{
		case (0x07):
		{
			//LD Vx, DT

			Registers[VX] = DelayRegister;

			break;
		}
		case (0x0A):
		{
			//LD Vx, K
			//to do
			Registers[VX] = 0;

			break;
		}
		case (0x15):
		{
			//LD DT, Vx

			DelayRegister = Registers[VX];

			break;
		}
		case (0x18):
		{
			//LD ST, Vx

			SoundTimerRegister = Registers[VX];

			break;
		}
		case (0x1E):
		{
			//ADD I, vx

			IRegister += Registers[VX];

			break;
		}
		case (0x29):
		{
			//LD F, Vx
			//to do

			//Set I to location of character Vx in mem

			break;
		}
		case (0x33):
		{
			//LD B, Vx
			//to do

			//Take address from i, write decimal representation of Vx in I, I+1, I+2

			break;
		}
		case (0x55):
		{
			//LD [I], Vx
			//to do

			//Write registers V0-Vx at location I 

			break;
		}
		case (0x65):
		{
			//LD Vx, [I]
			//to do

			//Set registers V0-Vx to values from location I

			break;
		}
		default:
			OP_NOP();
			break;
		}
	}
	}
}

void OP_NOP()
{
	printf("Hit no-operation opcode\n");
}