#include <stdio.h>
#include <stdlib.h>
#include "cpu.h"
#include "memory.h"
#include "instructions.h"

u16 destination;
u16 instruction;

u16 pc = 0; // program counter

#ifdef NOROM
u16 fake_rom[] = {
	// _mov = 0xfb
	// _add = 0xfa
	// _prn = 0xfc
	// _eoq = 0xfd
	
	_add, 0x0010,         // ADD 0x0010
	//_mov, 0x0010, 0x0014,
	//_prn, 0x0010, 0x0011,  // PRN 0x0010 → 0x0011
	//_eoq                  // EOQ
};
#endif

#ifndef NOROM

	static inline int add () {
		printf("Enter the address to increment:");	
		scanf("%x", &address);
		printf("Old value: 0x%04x $%04x \n", total_memory[address], total_memory[address]);
		total_memory[address] += 1;
		printf("Current value: 0x%04x $%04x \n", total_memory[address], total_memory[address]);		}

	static inline int mov () {
		printf("Enter the address and the destination:");
		scanf("%x %x", &address, &destination);
		printf("Old value: 0x%04x $%04x \n", total_memory[address], total_memory[address]);
		total_memory[address] = destination;
		printf("Current value: 0x%04x $%04x \n", total_memory[address], total_memory[address]);
	}

	static inline int prn () {
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

static inline int add() {
	address = fake_rom[++pc];
	total_memory[address] += 1;
	printf("%x \n", total_memory[address]);
	return address;
}

static inline int mov() {
	address = fake_rom[++pc];
	destination = fake_rom[++pc];
	total_memory[address] = destination;
	printf("%x \n", total_memory[address]);
	return address;
}

static inline int prn() {
	//address = fake_rom[pc];
	address	= fake_rom[++pc];
	end = fake_rom[pc + 1];
	for(u16 i = address; i < end; i ++) {
		printf("HUMAN: 0x%04x | SNES: $%04x \n", total_memory[i], total_memory[i]);
	}
}
#endif
