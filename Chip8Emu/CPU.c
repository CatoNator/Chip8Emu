#include "CPU.h"
#include "RAM.h"
#include "Display.h"
#include "Backend.h"
#include "Input.h"

uint8_t Registers[REGISTER_COUNT];

uint8_t DelayRegister = 0;
uint8_t SoundTimerRegister = 0;

uint16_t IndexRegister = 0;

uint8_t WaitingForInput = 0xFF;

void CPUInit()
{
	memset(&Registers, 0, STACK_SIZE);

	ProgramCounter = PROGRAM_START;

	DelayRegister = 0;
	SoundTimerRegister = 0;
	IndexRegister = 0;
}

void CPUTimerTick()
{
	if (DelayRegister > 0)
		DelayRegister--;

	if (SoundTimerRegister > 0)
		SoundTimerRegister--;
}

uint8_t RandVal = 0x80;

uint8_t Rand()
{
	uint8_t S0 = RandVal << 4;
	S0 = S0 ^ RandVal;

	RandVal = ((S0 & 0x0F) << 4) | ((S0 & 0xF0) >> 4);

	S0 = ((RandVal & 0x0F) << 1) ^ RandVal;

	uint8_t S1 = (S0 >> 1) ^ 0xF8;

	if ((S0 & 0x1) == 1)
		RandVal = 0x37 ^ S1;
	else
		RandVal = 0x23 ^ S1;

	return RandVal;
}

void ExecuteOpcode()
{
	//if opcode Fx0A has been tripped, we execute this instead of a normal opcode
	if (WaitingForInput != 0xFF)
	{
		if (InputBuffer != 0)
		{
			//Found input; scan buffer for what key it is
			for (uint8_t i = 0; i < 16; i++)
			{
				if ((1 << i) & InputBuffer)
				{
					Registers[WaitingForInput] = i;
					break;
				}
			}

			WaitingForInput = 0xFF;
		}
		
		return;
	}
	
	uint16_t OpCode = ReadShort(ProgramCounter);
	ProgramCounter += 2;

	uint8_t VX = (OpCode >> 8) & 0x000F; //Lower nibble of high byte
	uint8_t VY = (OpCode >> 4) & 0x000F; //Upper nibble of low byte
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
			OP_NOP(OpCode);
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
		switch (N)
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

			//carry bit
			Registers[0xF] = (Registers[VX] + Registers[VY] > 0xFF) ? 1 : 0;

			Registers[VX] += Registers[VY];

			break;
		}
		case (5):
		{
			//SUB Vx, Vy

			//weird bit thingy
			Registers[0xF] = (Registers[VX] > Registers[VY]) ? 1 : 0;

			Registers[VX] -= Registers[VY];

			break;
		}
		case (6):
		{
			//SHR Vx

			//LSB storage
			Registers[0xF] = (Registers[VX] & 0x01) ? 1 : 0;

			Registers[VX] >>= 1;

			break;
		}
		case (7):
		{
			//SUBN Vx, Vy

			//weird bit thingy - inverse of 0x8xy5
			Registers[0xF] = (Registers[VY] > Registers[VX]) ? 1 : 0;

			Registers[VX] = Registers[VY] - Registers[VX];

			break;
		}
		case (0xE):
		{
			//SHL Vx

			//MSB storage
			Registers[0xF] = (Registers[VX] & 0x80) ? 1 : 0;

			Registers[VX] <<= 1;

			break;
		}
		default:
			OP_NOP(OpCode);
			break;
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

		IndexRegister = ADDR;

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
		//RND Vx, byte

		Registers[VX] = Rand() & VAL;

		break;
	}
	case (0xD000):
	{
		//DRW Vx, Vy, n

		DrawSprite(Registers[VX], Registers[VY], (uint8_t*)(&Heap[IndexRegister]), N);

		break;
	}
	case (0xE000):
	{
		switch (VAL)
		{
		case (0x9E):
		{
			//SKP Vx

			if (IsButtonDown(Registers[VX]))
				ProgramCounter += 2;

			break;
		}
		case (0xA1):
		{
			//SKNP Vx
			if (!IsButtonDown(Registers[VX]))
				ProgramCounter += 2;

			break;
		}
		default:
			OP_NOP(OpCode);
			break;
		}
		break;
	}
	case (0xF000):
	{
		switch (VAL)
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
			//wait until a button is pressed; save into VX

			//Store register value in WaitForInput
			WaitingForInput = Registers[VX];
			return;

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

			//This isn't in all specs. I dunno if it causes any issues.
			//Registers[0xF] = (IndexRegister + Registers[VX] > 0x0FFF) ? 1 : 0;

			IndexRegister += Registers[VX];

			break;
		}
		case (0x29):
		{
			//LD F, Vx
			//Set IndexRegister to point to hexadecimal character of value in Vx

			IndexRegister = FONT_CHAR_SIZE * Registers[VX];

			break;
		}
		case (0x33):
		{
			//LD B, Vx
			//to do

			//Take address from i, write decimal representation of Vx in I, I+1, I+2
			Heap[IndexRegister]     = (Registers[VX] % 1000) / 100;
			Heap[IndexRegister + 1] = (Registers[VX] % 100) / 10;
			Heap[IndexRegister + 2] = (Registers[VX] % 10);

			break;
		}
		case (0x55):
		{
			//LD [I], Vx

			memcpy(&Heap[IndexRegister], &Registers, VX + 1);

			break;
		}
		case (0x65):
		{
			//LD Vx, [I]

			//Set registers V0-Vx to values from location I

			memcpy(&Registers, &Heap[IndexRegister], VX + 1);

			break;
		}
		default:
		{
			OP_NOP(OpCode);
			break;
		}
		}
		break;
	}
	default:
		OP_NOP(OpCode);
		break;
	}
#ifdef _DEBUG
	printf("OP %x VX %u VY %u N %u VAL %u ADDR %u\n", OpCode, VX, VY, N, VAL, ADDR);
#endif
}

static void DumpRegisters()
{
	printf("\n---- BEGIN REGISTER DUMP ----\n\n");

	printf("REG 0 %u | REG 1 %u | REG 2 %u | REG 3 %u\n\
REG 4 %u | REG 5 %u | REG 6 %u | REG 7 %u\n\
REG 8 %u | REG 9 %u | REG A %u | REG B %u\n\
REG C %u | REG D %u | REG E %u | REG F %u\n",
		Registers[0x0], Registers[0x1], Registers[0x2], Registers[0x3],
		Registers[0x4], Registers[0x5], Registers[0x6], Registers[0x7],
		Registers[0x8], Registers[0x9], Registers[0xA], Registers[0xB],
		Registers[0xC], Registers[0xD], Registers[0xE], Registers[0xF]);

	printf("\n I %u\nDT %u\nST %u\n\nProgramCounter %u\nStackPointer %u\n", IndexRegister, DelayRegister, SoundTimerRegister, ProgramCounter, StackPointer);

	printf("\n\n---- END REGISTER DUMP ----\n");
}

void OP_NOP(uint16_t OpCode)
{
	printf("Hit no-operation opcode %x\n", OpCode);

	DumpRegisters();

	BackendPanic("Unknown opcode!");
}