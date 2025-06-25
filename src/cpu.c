#include "defs.h"
#include "instructions.h"

#include <stdio.h>
#include <stdbool.h>
#include <unistd.h>

bool cpu_waiting;
u16 instruction;
int address;

u16 total_memory[65536];

static int init_cpu() {
	printf("We did nothing lmao \n");
	return 0;
}

static inline int exec_instruction() {
	printf("Enter the address:");	
	// this is the equivalent to ADD instruction
	scanf("%d", &address);
	if (address >= (sizeof(total_memory) / sizeof(total_memory[0]))) {
		printf("WARNING! This value exceeds memory and cannot be used! \n");
		return 1;
	}
	printf("Before: 0x%04X \n", total_memory[address]);
	total_memory[address] += address;
	printf("Now: 0x%04X \n", total_memory[address]);
	return 0;
}

extern inline void mainFunc() {
	while (1) {
		printf("Waiting for instruction... \n");
		exec_instruction();
		break;
	}	
	return;
}

static inline void printMemory() {
	for(u16 i = 0; i < (sizeof(total_memory) / sizeof(total_memory[0])); i ++) {
		printf("Address number: %04x \n", total_memory[i]);
		sleep(1);
	}
}
