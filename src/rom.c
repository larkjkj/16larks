#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "arguments.h"
#include "cpu.h"
#include "rom.h"

size_t rom_size = 0;
char* rom_type;
u8 header_addr;

extern inline void splitRom(char* name) {
	FILE* rom_file = fopen(name, "rb");

	if (rom_file == NULL) {
		printf("Error: rom file is corrupted or non-existent \n");
	}
	if (fseek(rom_file, 0, SEEK_END) == 0) {
		rom_size = ftell(rom_file);
		if (rom_size == 0) {
			printf("ROM size is 0? Rom buffer will not be specified correctly \n");
		} else {
			rewind(rom_file);
			rom = (malloc(rom_size));
			fread(rom, sizeof(u8), rom_size, rom_file);
			if (rom_size % 1024 == 512) {
				rom += 0x200;
			}

			printf("ROM size: %i \n", rom_size);
			if (verbose) {
				for(int i = 0; i < rom_size; i++) {
					printf("Printing address: %04X \n", rom[i]);
				}
			}
		}
	}	
/*	if (fread(rom, sizeof(rom_size), (sizeof(rom_size) / sizeof(u16)), rom_file)) {
		printf("Sucess reading the rom \n");
		printf("ROM SIZE: %i \n", sizeof(rom_file));
	} else {
		printf("Fail opening the rom file, wrong name? \n");
		exit(1);
	}*/
	//printf("Printing address: %i \n", rom[0x7fd5]);

	header_addr = rom[0xffc0];
	if (header_addr < 20 && header_addr > 35) {
		header_addr = rom[0x7fc0];
	}

	switch (header_addr) {
		case 20:
			rom_type = "lorom";
			pc = 0x8000;
		break;
		case 21:
			rom_type = "hirom";
		break;
		case 23:
			rom_type = "sa-1";
		break;
		case 30:
			rom_type = "(fastrom) lorom";
		break;
		case 31:
			rom_type = "(fastrom) hirom";
		break;
		case 32:
			rom_type = "sdd-1 rom";
		break;
		case 35:
			rom_type = "exhirom";
		break;
		default:
			rom_type = "unknown";
		break;
	}
	
	printf("Value from rom_type is: %s %d \n", rom_type, header_addr);
	sleep(3);

	// identify betw lo and hi
	//
//	0x0000 - 0x003F
//	0x07D0 - 0x203F
	
//	0x176F - 
	fclose(rom_file);
	return;
}
