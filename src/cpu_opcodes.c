#include <stdio.h>
#include <stdlib.h>

#include "rom.h"
#include "cpu.h"
#include "cpu_opcodes.h"
#include "memory.h"
#include "instructions.h"
#include "arguments.h"

#ifdef DEBUG

inline void add() {
	printf("Enter the address to increment:");	
	scanf("%x", &address);
	printf("Old value: 0x%04x $%04x \n", total_memory[address], total_memory[address]);
	total_memory[address] += 1;
	printf("Current value: 0x%04x $%04x \n", total_memory[address], total_memory[address]);		}

inline void mov() {
	printf("Enter the address and the destination:");
	scanf("%x %x", &address, &destination);
	printf("Old value: 0x%04x $%04x \n", total_memory[address], total_memory[address]);
	total_memory[address] = destination;
	printf("Current value: 0x%04x $%04x \n", total_memory[address], total_memory[address]);
}

inline void prn() {
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

inline void ato() {
	printf("Select the start and the end address: ");
	scanf("%x, %x", &address, &end);
	printf("%c %c", &address, &end);
}

#else

inline void add(){
	address = rom[pc++];
	total_memory[address] += 1;
	if (verbose) {
		printf("Value is %x \n", total_memory[address]);
	}
}

inline void mov(){
	address = rom[pc++];
	destination = rom[pc++];
	total_memory[address] = destination;
	if (verbose) {
		printf("%x \n", total_memory[address]);
	}
	//return address;
}

inline void prn() {
	address	= rom[pc++];
	end = rom[pc++];
	if (verbose) {
		for(u16 i = address; i < end; i ++) {
			printf("HUMAN: 0x%04x | SNES: $%04x \n", total_memory[i], total_memory[i]);
		}
	}
}

inline void jmp() {
	// probally more advanced than that
//	pc = address[rom];
	//
	//	yea if course this shit is;
	//if (address[rom] == cpu_instructions)
	//	pc = (int)cpu_instructions;
	//else
	pc = total_memory[++pc];
	//address = rom[pc++];
}

inline void ato() {
	address = rom[++pc];
	end = rom[pc++];
	for(u16 i = address; i < end; i ++) {
		printf("%xc \n", rom[i]);
	}
}
#endif

