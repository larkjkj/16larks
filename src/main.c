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

u8* rom = NULL;
int norom = 0;
int verbose = 0;

#ifdef _EE
#include <unistd.h>
#include <debug.h>
#include <kernel.h>
#include <ps2_filesystem_driver.h>
#endif

int main(int argc, char* argv[]) {
	initGraphics();
	for(int i = 0; i < argc; i += 1) {
		if (strcmp(argv[i], "--verbose") == 0) {
			verbose = 1;
		} else if (strcmp(argv[i], "--rom") == 0) {
			//this code here was used because i was struggling
			//with just defining the rom name, so i rewrite it
			//and it works(nice programmer btw)
			//strncpy(rom_name, argv[++i], sizeof(argv[++i]));
			#ifdef _EE
			rom_name = (char *) "roms/zelda3.sfc";
			splitRom(rom_name);
			#else
			rom_name = argv[++i];
			printf("%s \n", rom_name);
			splitRom(rom_name);
			#endif
		}
	}
	if (rom == NULL) {
		printf("Null \n");
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

