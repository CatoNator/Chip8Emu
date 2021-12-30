#include "Sound.h"
#include "CPU.h"

#define PI (3.14159f)

float TimeStep = 1.0f / (float)SAMPLE_RATE;
float GlobalTime = 0.0f;

int16_t GetWave(float Time)
{
	float Sine = (float)(sin(440.0f * PI * Time)); //Sine wave at A4
	
	return Sine * 0x0FFF;
}

void FillSoundBufferMono(int16_t* Buffer, uint32_t Buffersize)
{
	uint32_t i = 0;

	while (i < Buffersize)
	{
		if (SoundTimerRegister > 0)
		{
			Buffer[i] = GetWave(GlobalTime);
			GlobalTime += TimeStep;
		}
		else
		{
			Buffer[i] = 0;
			GlobalTime = 0.0f;
		}

		i++;
	}
}

void FillSoundBufferStereo(int16_t* Buffer, uint32_t Buffersize)
{
	uint32_t i = 0;

	while (i < Buffersize)
	{
		int16_t Sample = 0;

		if (SoundTimerRegister > 0)
		{
			Sample = GetWave(GlobalTime);
			GlobalTime += TimeStep;
		}
		else
			GlobalTime = 0.0f;
		
		Buffer[i] = Sample;
		Buffer[i + 1] = Sample;
		i += 2;
	}
}