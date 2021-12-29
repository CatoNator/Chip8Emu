#include "Backend.h"

/*
This backend implementation is written for windows using SDL2.

Any other device-specific implementations can be written by overwriting this file
with code and library includes specific for the device you're targeting.
*/

#include <SDL.h>

uint8_t FrameAdv = 0;

// ---- GRAPHICS DEFS AND VARS ---- //

SDL_Window* gGameWindow = NULL;

SDL_Renderer* gGameRenderer = NULL;

SDL_Texture* gGameBackBuffer = NULL;

int gBackBufferWidth = 256;
int gBackBufferHeight = 128;

// ---- INPUT DEFS AND VARS ---- //

// ---- SOUND DEFS AND VARS ---- //

// ---- BACKEND.H MANDATED IMPLEMENTATIONS ---- //

uint8_t Running = 0;

void BackendInit()
{
	//configure DPI stuff
	//SetDPIAware();

	//SDL_Initialization
	if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO | SDL_INIT_JOYSTICK) < 0)
	{
		printf("SDL INITIALIZATION ERROR %s\n", SDL_GetError());
		return;
	}

	//create a window
	gGameWindow = SDL_CreateWindow("Test", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, gBackBufferWidth, gBackBufferHeight, SDL_WINDOW_SHOWN | SDL_WINDOW_OPENGL);

	if (gGameWindow == NULL)
	{
		printf("Failed creating window! Error %s\n", SDL_GetError());
		return;
	}

	//create a renderer
	gGameRenderer = SDL_CreateRenderer(gGameWindow, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_TARGETTEXTURE);

	if (gGameRenderer == NULL)
	{
		printf("Failed creating renderer! Error %s\n", SDL_GetError());
		return;
	}

	//the target texture onto which the game is drawn in
	gGameBackBuffer = SDL_CreateTexture(gGameRenderer, SDL_GetWindowPixelFormat(gGameWindow), SDL_TEXTUREACCESS_STREAMING, RES_H, RES_V);

	if (gGameBackBuffer == NULL)
	{
		printf("Failed creating backbuffer texture! Error %s\n", SDL_GetError());
		return;
	}

	//Renderer default colour(black)
	SDL_SetRenderDrawColor(gGameRenderer, 0x00, 0x00, 0x00, 0xFF);

	//Configure emulator
	InitRAM();

	//Init screen
	ClearScreen();

	//Set CPU to initial state
	CPUInit();

	Running = 1;
}

uint8_t BackendLoadROM(const char* Path)
{
	FILE* RomFile = fopen(Path, "rb+");

	if (RomFile == NULL)
	{
		printf("Failed to open file %s!\n", Path);
		return 0;
	}

	if (Heap == NULL)
	{
		printf("RAM uninitialized!\n");
		return 0;
	}

	//Seek to EOF
	fseek(RomFile, 0L, SEEK_END);

	//Get filesize
	uint32_t FileSize = ftell(RomFile);

	printf("Filesize %u\n", FileSize);

	//Seek to BOF
	fseek(RomFile, 0L, SEEK_SET);

	//Copy ROM to mem
	fread((void*)(Heap + PROGRAM_START), 1, FileSize, RomFile);

	fclose(RomFile);

	RomFile == NULL;
	
	return 1;
}

void BackendPollInput()
{
	SDL_Event e;

	while (SDL_PollEvent(&e) != 0)
	{
		//User requests quit
		if (e.type == SDL_QUIT || (e.type == SDL_KEYDOWN && e.key.keysym.sym == SDLK_ESCAPE))
		{
			Running = 0;
		}
		//idk what to do with inputs yet here
		else if (e.type == SDL_KEYDOWN)
		{
			//Temp keyboard polling
			if (e.key.keysym.sym == SDLK_SPACE)
				FrameAdv = 1;
		}
		else if (e.type == SDL_KEYUP)
		{
				
		}
	}
}

void BackendRun()
{
	uint32_t Ticks = SDL_GetTicks();
	uint32_t PrevTicks = Ticks;
	uint32_t TickDelta = 0;

	while (Running)
	{
		
		
		//Poll input
		BackendPollInput();

		//emulate CPU cycle
		ExecuteOpcode();

		//Draw buffer to screen
		BackendFinalDraw();

		//DT and ST ticks at 60Hz
		Ticks = SDL_GetTicks();
		TickDelta = Ticks - PrevTicks;

		if (TickDelta > 1000.0f/(float)CPU_TICK_RATE)
		{
			PrevTicks = Ticks;

			CPUTimerTick();
		}
	}
}

void BackendPanic(const char* Message)
{
	printf("Panic! %s\n", Message);

	Running = 0;
#ifdef _DEBUG

	printf("---- BEGIN HEAP DUMP ----\n");

	//Dump heap
	for (int i = 0; i < RAM_SIZE; i++)
	{
		printf("%x ", Heap[i]);
	}

	printf("\n---- END HEAP DUMP ----\n");

	/*FILE* Dump = fopen("Heap.bin", "wb+");

	fwrite(Heap, 1, RAM_SIZE, Dump);

	fclose(Dump);*/

#endif // !_DEBUG
}

void BackendFinalDraw()
{
	//Rendering CHIP-8 buffer to SDL texture
	uint32_t* TextureBuffer;
	uint32_t TextureFormat;

	int TextureWidth = 0;
	int TextureHeight = 0;

	int Pitch = 0;

	SDL_QueryTexture(gGameBackBuffer, &TextureFormat, NULL, &TextureWidth, &TextureHeight);

	uint32_t TextureSize = (uint32_t)(TextureWidth * TextureHeight);

	if (TextureSize != (RES_H * RES_V))
	{
		printf("Framebuffer size mismatch! TSIZE %u vs FBUFSIZE %u\n", TextureSize, (RES_H * RES_V));
		return;
	}

	SDL_PixelFormat* format = SDL_AllocFormat(TextureFormat);

	if (SDL_LockTexture(gGameBackBuffer, NULL, (void**)&TextureBuffer, &Pitch) != 0)
	{
		printf("Unable to lock texture! %s\n", SDL_GetError());
		return;
	}
	
	for (int v = 0; v < RES_V; v++)
	{
		for (int h = 0; h < RES_H; h++)
		{
			if (DisplayBuffer[v] & (ZERO_PIX >> h))
				TextureBuffer[v * RES_H + h] = SDL_MapRGBA(format, 0xFF, 0xFF, 0xFF, 0xFF);
			else
				TextureBuffer[v * RES_H + h] = SDL_MapRGBA(format, 0x00, 0x00, 0x00, 0xFF);
		}
	}

	SDL_UnlockTexture(gGameBackBuffer);

	//Draw texture to screen

	//clear screen
	SDL_RenderClear(gGameRenderer);

	//draw backbuffer to screen
	SDL_Rect NativeRes = { 0, 0, RES_H, RES_V }; //Native res size rect
	SDL_Rect TargetRes = { 0, 0, gBackBufferWidth, gBackBufferHeight }; //window size rect
	int result = SDL_RenderCopy(gGameRenderer, gGameBackBuffer, &NativeRes, &TargetRes);

	if (result != 0)
	{
		printf("SDL drawing error %s\n", SDL_GetError());
	}

	SDL_RenderPresent(gGameRenderer);
}

void BackendExit()
{
	//Close emulator systems
	CleanRAM();
	
	//Close audio device
	//SDL_CloseAudioDevice(AudioDevice);

	//SDL_AudioQuit();

	//Cleanup SDL
	SDL_DestroyTexture(gGameBackBuffer);
	SDL_DestroyWindow(gGameWindow);
	SDL_DestroyRenderer(gGameRenderer);

	gGameBackBuffer = NULL;
	gGameWindow = NULL;
	gGameRenderer = NULL;

	//Quit SDL subsystems
	SDL_Quit();
}