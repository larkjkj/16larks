#include <stdio.h>
#include <stdlib.h>
#include "cpu.h"
#include "memory.h"

#ifndef NOROM
	static inline void add () {
		printf("Enter the address to increment:");	
		scanf("%x", &address);
		printf("Old value: 0x%04x $%04x \n", total_memory[address], total_memory[address]);
		total_memory[address] += 1;
		printf("Current value: 0x%04x $%04x \n", total_memory[address], total_memory[address]);				
	}
	static inline void mov () {
		printf("Enter the address and the destination:");
		scanf("%x %x", &address, &destination);
		printf("Old value: 0x%04x $%04x \n", total_memory[address], total_memory[address]);
		total_memory[address] = destination;
		printf("Current value: 0x%04x $%04x \n", total_memory[address], total_memory[address]);
	}
	static inline void prn () {
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
	static inline int add(u16 address) {
		total_memory[address] += 1;
		printf("%x", total_memory[address]);
		return address;
	}

	static inline int mov(u16 address, u16 destination) {
		total_memory[address] = destination;
		printf("%x", total_memory[address]);
		return address;
	}
	extern inline void prn() {
		printf("Instruction is debug-only \n");
	}
#endif
