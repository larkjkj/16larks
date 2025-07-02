#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "arguments.h"

#include "funcs.h"
#include "rom.h"
#include "rom_funcs.h"
#include "init_funcs.h"

int norom = 0;
int verbose = 0;
u16 rom[] = {};
char* tmp_rom;

int main(int argc, char* argv[]) {
	initGraphics();
	for(int i = 0; i < argc; i += 1) {
		if (strcmp(argv[i], "--verbose") == 0) {
			verbose = 1;
		}
		if (strcmp(argv[i], "--norom") == 0) {
			norom = 1;
			u16 rom[sizeof(fake_rom)];
			memcpy(rom, fake_rom, sizeof(fake_rom));
		} else if (strcmp(argv[i], "--rom=") == 0) {
			tmp_rom = argv[i+6];
			splitRom(tmp_rom);
		} else {
			u16 rom[] = {};
		}
	}
	identifyRom();
	//splitRom(argv[1]);
	printf("Slow to how many cycles per second:");
	scanf("%i", &cycle);
	printf("Verbose is %d \n", verbose);
	printf("NoRom is %d \n", norom);
	while (1) {
		//printMemory();
		if (mainFunc() > 0) {
			printf("Return code is not 0, something is wrong \n");
		}
		break;
	}
	return 0;
}
