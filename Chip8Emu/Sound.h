#pragma once
#ifndef SOUND_H
#define SOUND_H

#include <stdint.h>

#define SAMPLE_RATE (44100)
#define AUDIO_CHANNELS (1)

extern void FillSoundBufferMono(int16_t* Buffer, uint32_t Buffersize);
extern void FillSoundBufferStereo(int16_t* Buffer, uint32_t Buffersize);

#endif