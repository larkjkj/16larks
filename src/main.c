#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <errno.h>

#include "arguments.h"

#include "cpu.h"
#include "funcs.h"
#include "rom.h"
#include "init_funcs.h"
#include "memory.h"

int norom = 0;
u8* rom = NULL;
int verbose = 0;

#ifdef _PS2
#include <unistd.h>
#include <debug.h>
#include <kernel.h>
char* rom_name = "roms/zelda3.smc";
#else
char* rom_name;
#endif

int main(int argc, char* argv[]) {
	initGraphics();
	// Just to check is allocation was succesfully to avoid SEGFAULT
	if (rom != NULL)
		printf("Sucess! %p \n", rom[pc]);
	
	for(int i = 0; i < argc; i += 1) {
		if (strcmp(argv[i], "--verbose") == 0) {
			verbose = 1;
		} else if (strcmp(argv[i], "--norom") == 0) { 
			norom = 1;
			memcpy(rom, fake_rom, sizeof(fake_rom));
			rom_name = "USING FAKEROM FROM DEF";
		} else if (strcmp(argv[i], "--rom") == 0) {
			//this code here was used because i was struggling
			//with just defining the rom name, so i rewrite it
			//and it works(nice programmer btw)
			//strncpy(rom_name, argv[++i], sizeof(argv[++i]));
			rom_name = argv[++i];
			printf("%s \n", rom_name);
			splitRom(rom_name);
		}
	}
	//printf("Verbose is %d \n", verbose);
	//printf("NoRom is %d \n", norom);
	while (1) {
		printMemory();
		mainFunc();
		break;
	}
	return 0;
}
