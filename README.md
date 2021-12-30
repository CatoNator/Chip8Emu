# Chip8Emu
 a CHIP-8 emulator, written in C using VS2019. For fun, mostly.
 
 The emulator is currently functional, but not the most optimized. There probably are hundreds of bugs lurking inside as well.
 
 Compilation:
 
 The implementation includes a Backend.c written for Windows with SDL2. This file will have to be rewritten to port the implementation to other backends or platforms.
 
 On little-endian devices, add SYS_LENDIAN as a preprocessor directive.
