#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "arguments.h"
#include "cpu.h"
#include "rom.h"

size_t rom_size = 0;
char* rom_type = "unknown";
int w_header;
u32 header_addr;

/* There is an explanation why the addresses have defined use
 * It's because SNES have a maximum ROM size of 4MB(without 
 * any special chips), so we just use that */

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
			printf("ROM size: %d \n", rom_size);
			rewind(rom_file);
			rom = (malloc(rom_size));
			fread(rom, sizeof(u8), rom_size, rom_file);
			if (rom_size % 1024 == 512) {
				offset = 0x200;
			} else {
				offset = 0;
			}
		}
	}	
	/* A way the rom type is checking the ROM Size, and checking the
	 * value in that address */
	//low rom = 0x007fc0
	//hi rom = 0x00ffc0
	//exhi rom = 0x40ffc0

	u32 mem_address[3] = {
		0x007fc0,
		0x00fdc0,
		0x00ffc0,
		//(sizeof(rom_size) >= (4096 * 1000) ? 0x40ffc0 : 0x00),
	};
	for(int i = 0; i < (sizeof(mem_address) / sizeof(mem_address[0])); ++i) {
		header_addr = (rom[offset + mem_address[i]]);
		if (header_addr < 20 || header_addr > 35) {
			printf("No valid found %x \n", header_addr);
			continue;
		} else {
			break;
		}
	}
	
	switch (header_addr) {
		case 20:
			rom_type = "lorom";
			pc = rom[offset + 0x8000];
		break;
		case 21:
			rom_type = "hirom";
		break;
		case 23:
			rom_type = "sa-1";
			pc = offset + 0x8000;
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
	sleep(2);
	// identify betw lo and hi
	//
//	0x0000 - 0x003F
//	0x07D0 - 0x203F
	
//	0x176F - 
	fclose(rom_file);
	return;
}
