#include <stdio.h>
#include "defs.h"

// gpt gave a good reason to think lmao
enum emulator_instructions {
	_add = 0xfa, // add 
	_mov = 0xfb,
	_prn = 0xfc,
	_eoq = 0xfd,
};

enum cpu_instructions {
	_adc = 0x61, 
	_beq = 0xf0,
	_lda = 0xa1,
	_sta = 0x8d,
	_sbc = 0xe1,
	_jmp = 0x4c,
	_jsr = 0x20,
};

extern u16 destination;
extern u16 instruction;
extern u16 pc;


