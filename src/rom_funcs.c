#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "arguments.h"
#include "rom.h"

size_t rom_size = 0;

extern inline void identifyRom() {
	if (norom) {
		//for(int i = 0; i < sizeof(fake_rom); i ++) {
		//	printf("%x \n", fake_rom[i]);
		//}
	}
}

extern inline void splitRom(char* name) {
	FILE* rom_file = fopen(name, "rb");
	if (fseek(rom_file, 0, SEEK_END) == 0) {
		size_t rom_size = ftell(rom_file);
		rewind(rom_file);

		if (rom_size == 0) {
			printf("ROM size is 0? Rom buffer will not be specified correctly \n");
		} else {
			printf("ROM size: %i \n", rom_size);
		}
	} else {
		printf("Error moving pointer to beggining \n");
	}
	
	u16* rom = (malloc(rom_size));
	printf("ROM array %i \n", rom);
		if (fread(rom, sizeof(u8), (sizeof(rom_size)), rom_file)) {
		printf("Sucess reading the rom \n");
		printf("ROM SIZE: %i", sizeof(rom_file));
	} else {
		printf("Fail opening the rom file, wrong name? \n");
		memcpy(rom, fake_rom, sizeof(fake_rom));
	}

	// identify betw lo and hi
	//
//	0x0000 - 0x003F
//	0x07D0 - 0x203F
	
//	0x176F - 

	if (rom_file == NULL) {
		printf("Error: rom file is corrupted or non-existent \n");
	}
	fclose(rom_file);
	return;
}
