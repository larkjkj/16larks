#include <stdio.h>
#include <string.h>
#include "arguments.h"
#include "rom.h"

void identifyRom() {
	if (norom) {
		memcpy(rom, fake_rom, (sizeof(fake_rom) / sizeof(fake_rom[0])));
		for(int i = 0; i < sizeof(rom); i ++) {
			printf("%x \n", rom[i]);
		}
	}
}

inline void splitRom(const char* rom) {
	return;
}
