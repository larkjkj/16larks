#include <stdio.h>
#include <stdlib.h>

#include "cpu_opcodes.h"
#include "memory.h"
#include "instructions.h"
#include "arguments.h"

u16 address;
u16 total_memory[];
u16 destination;
u16 instruction;
u16 pc; // program counter

/*
u16 fake_rom[] = {
	// MOV 0x0011 → [0x0000]
	_mov, 0x0000, 0x0011,

	// MOV 0x000A → [0x0001]
	_mov, 0x0001, 0x000A,

	// MOV 0x00FF → [0x0002]
	_mov, 0x0002, 0x00FF,

	// ADD [0x0001] (valor 0x000A → vira 0x000B)
	_add, 0x0001,

	// ADD [0x0002] (0x00FF → 0x0100)
	_add, 0x0002,

	// PRN 0x0000 até 0x0003
	_prn, 0x0000, 0x0003,

	// MOV 0x1234 → [0x0010]
	_mov, 0x0010, 0x1234,

	// ADD [0x0010] → 0x1235
	_add, 0x0010,

	// PRN 0x0010 até 0x0012
	_prn, 0x0010, 0x0012,

	// Finaliza
	_eoq
};
/*u16 fake_rom[] = {
	// _mov = 0xfb
	// _add = 0xfa
	// _prn = 0xfc
	// _eoq = 0xfd
	_add, 0x0010,
	_mov, 0x0010, 0x0014,
	_prn, 0x0010, 0x0011,
	_eoq,

	_add, 0x0010,         // ADD 0x0010
	_mov, 0x0010, 0x0014,
	_prn, 0x0010, 0x0011,  // PRN 0x0010 → 0x0011
	_eoq                  // EOQ
};*/

#ifdef DEBUG
	extern inline int add () {
		printf("Enter the address to increment:");	
		scanf("%x", &address);
		printf("Old value: 0x%04x $%04x \n", total_memory[address], total_memory[address]);
		total_memory[address] += 1;
		printf("Current value: 0x%04x $%04x \n", total_memory[address], total_memory[address]);		}

	extern inline int mov () {
		printf("Enter the address and the destination:");
		scanf("%x %x", &address, &destination);
		printf("Old value: 0x%04x $%04x \n", total_memory[address], total_memory[address]);
		total_memory[address] = destination;
		printf("Current value: 0x%04x $%04x \n", total_memory[address], total_memory[address]);
	}

	extern inline int prn () {
		printf("Select the start and the end address: ");
		scanf("%x %x", &address, &end);
		if (end < address) {
			printf("Error, end is smaller than start \n");
			exit(1);
		}
		for(u16 i = address; i < end; i ++) {
			printf("HUMAN: 0x%04x | SNES: $%04x \n", total_memory[i], total_memory[i]);
		}
	}

#else
extern inline int add() {
	address = rom[pc++];
	total_memory[address] += 1;
	if (verbose) {
		printf("Value is %x \n", total_memory[address]);
	}
	return address;
}

extern inline int mov() {
	address = rom[pc++];
	destination = rom[pc++];
	total_memory[address] = destination;
	if (verbose) {
		printf("%x \n", total_memory[address]);
	}
	return address;
}

extern inline int prn() {
	address	= rom[pc++];
	end = rom[pc++];
	if (verbose) {
		for(u16 i = address; i < end; i ++) {
			printf("HUMAN: 0x%04x | SNES: $%04x \n", total_memory[i], total_memory[i]);
		}
	}
}
#endif

