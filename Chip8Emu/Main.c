#include <stdio.h>
#include"Backend.h"

int main(int argc, char* args[])
{
    if (argc < 1)
    {
        printf("Usage: Chip8Emu.exe ROMPath");
        return 0;
    }
    
    BackendInit();

    //Read rom from command line, load it
    BackendLoadROM(args[1]);

    BackendRun();

    BackendExit();

    return 0;
}