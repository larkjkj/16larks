#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "arguments.h"
#include "cpu.h"
#include "rom.h"

size_t rom_size = 0;
char* rom_type = "unknown";
FILE* rom_file = NULL;
char* rom_header;
char* rom_name;
int w_header;

/* There is an explanation why the addresses have been defined use
 * It's because SNES(hirom and lorom) have a maximum ROM size 
 * of 4MB(without any special chips), so we just use that */

enum rom_type {
	LoRom	= 0x20,
	HiRom	= 0x21,
	SA_1	= 0x23,
	exHiRom	= 0x35,
	F_LoRom	= 0x30,
	F_HiRom	= 0x31,
	SDD_1	= 0x32,
};

extern inline void splitRom(char* name) {
	rom_file = fopen(name, "rb");

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
			rom = NULL;
			rom = (malloc(rom_size));
			fread(rom, (sizeof(u8)), rom_size, rom_file);

		}
	}	
	/* A way the rom type is checking the ROM Size, and checking the
	 * value in that address */
	//low rom = 0x007fc0
	//hi rom = 0x00ffc0
	//exhi rom = 0x40ffc0

	u32 mem_address[3] = {
		0x00007fc0,
		0x0000fdc0,
		0x0000ffc0,
	};
	for(int i = 0; i < (sizeof(mem_address) / sizeof(mem_address[0])); ++i) {
		/* Little hack, since our rom is an u8 bit, we need to make an
		 * address from 3 bytes, and get our value :) */
		
		/* Another mistake i made, i took a time to understand that the 
		 * first 21 bytes are the ROM NAME, and the after one is the rom_type 
		 * 0x54, 0x48, 0x45, 0x20, 0x4C, 0x45, 0x47, 0x45, ← ROM NAME
		 * 0x4E, 0x44, 0x20, 0x4F, 0x46, 0x20, 0x5A, 0x45, ← ROM NAME
		 * 0x4C, 0x44, 0x41, 0x20, [0x20] 		   ← [0x20] is our ROM_TYPE: LOROM */

		byte_low = 0x15 + mem_address[i << 8];
		byte_high = 0x15 + mem_address[i];
		/* In case of headered roms */
		if ((rom_size % 1024) == 512) {
			rom = &rom[((0xFF * 2) + 2)];
			//rom = &rom[((0x200))];
		}
		printf("%x \n", rom[byte_high] | rom[byte_low]);
		
		address = (rom[byte_high] | rom[byte_low]);
		
		if (address < 20 || address > 35) {
			printf("No valid found %x \n", address);
			continue;
		} else {
			printf("Found! %x \0", address);
			break;
		}
	}
	
	switch (address) {
		/* In some cases the layout changes, so we NEED to make this */
		case LoRom:	
			rom_type = "LOROM";
			address = (rom[0x80] | rom[0x00]);
			pc = address;
		break;
		case HiRom:
			rom_type = "HIROM";
			pc = offset + 0x8000;
			/* This is wrong btw, hi-rom is a lot different from lo-rom */
		break;
		case SA_1:
			rom_type = "SA-1";
			pc = offset + 0x8000;
			/* This Code will enable SA-1 Chip use */
		break;
		case F_LoRom:
			rom_type = "(fastrom) LOROM";
		break;
		case F_HiRom:
			rom_type = "(fastrom) HIROM";
		break;
		case SDD_1:
			rom_type = "sdd-1 rom";
		break;
		case exHiRom:
			rom_type = "exHIROM";
		break;
		default:
			rom_type = "UNKNOWN";
		break;
	}
	printf("Value from rom_type is: %s %x \n", rom_type, address);
	sleep(2);
	// identify betw lo and hi
	//
//	0x0000 - 0x003F
//	0x07D0 - 0x203F
	
//	0x176F - 
	fclose(rom_file);
	return;
}
