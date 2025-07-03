#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <errno.h>

#include "arguments.h"

#include "funcs.h"
#include "rom.h"
#include "rom_funcs.h"
#include "init_funcs.h"

int norom = 0;
char* rom_name;
int verbose = 0;
u16* rom = NULL;

int main(int argc, char* argv[]) {
	initGraphics();
	u16* rom = malloc(sizeof(u16));
	for(int i = 0; i < argc; i += 1) {
		if (strcmp(argv[i], "--verbose") == 0) {
			verbose = 1;
		}
		if (strcmp(argv[i], "--norom") == 0) { 
			norom = 1;
			memcpy(&rom, &fake_rom, sizeof(fake_rom));
			rom_name = "USING FAKEROM FROM DEF";
		} else if (strcmp(argv[i], "--rom") == 0) {
			//this code here was used because i was struggling
			//with just defining the rom name, so i rewrite it
			//and it works(nice programmer btw)
			//strncpy(rom_name, argv[++i], sizeof(argv[++i]));
			memcpy(&rom_name, &argv[++i], sizeof(rom_name));
			printf("%s \n", rom_name);
			splitRom(rom_name);
		} else {
			printf("Running CPU with no instructions \n");
		}
	}
	//printf("%i", sizeof(rom));
	//splitRom(argv[1]);
	//printf("Slow to how many cycles per second:");
	//scanf("%i", &cycle);
	//printf("Verbose is %d \n", verbose);
	//printf("NoRom is %d \n", norom);
	while (1) {
		printMemory();
		if (mainFunc() > 0) {
			printf("Return code is not 0, something is wrong \n");
		}

		//eventLoop();
		//this is if the mainFunc is causing problems, so comment the mainFunc
		//and uncomment this one
		break;
	}
	free(rom);
	return 0;
}
