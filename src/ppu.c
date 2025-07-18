/* File responsable for PPU stuff (graphics) */

#include <stdio.h>
#include "SDL_rwops.h"
#include "defs.h"
#include "SDL_surface.h"

u8 ppu_readbyte;
u8* ppu_addr_rd;
u16 ppu_cgram;
void* ppu_ptr;

void readGraphics(u8* address) {
	/* This function simply will convert a u8
	 * to a texture */

	return;
	/* Basically in a loop looking for the next 8-bit */
}
