#include <stdio.h>
#include <string.h>
#include "arguments.h"
#include "rom.h"

extern inline void identifyRom() {
	if (norom) {
		//for(int i = 0; i < sizeof(fake_rom); i ++) {
		//	printf("%x \n", fake_rom[i]);
		//}
	}
}

extern inline void splitRom(const char* rom) {
	FILE* rom_file;
	rom_file = fopen(rom, "r");
	
	fseek(rom_file, 0, SEEK_END);

	// identify betw lo and hi
	//
//	0x0000 - 0x003F
//	0x07D0 - 0x203F
	
//	0x176F - 

	if (rom_file == NULL) {
		printf("Error: rom file is corrupted or non-existent \n");
	}
	return;
}
