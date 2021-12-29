# Chip8Emu
 a CHIP-8 emulator, written in C using VS2019. For fun, mostly.
 
 Games run, but the implementation is still missing sound and input.
 
 Compilation:
 
 The implementation includes a Backend.c written for Windows with SDL2. This file will have to be rewritten to port the implementation to other backends or platforms.
 
 On little-endian devices, add SYS_LENDIAN as a preprocessor directive.
