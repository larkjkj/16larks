#include "defs.h"
#include "instructions.h"

#include <stdio.h>
#include <stdbool.h>
#include <unistd.h>
#include <string.h>

bool cpu_waiting;
u16 destination;
i16 instruction;

u16 address;
u16 end;
u16 total_memory[65536];

static int init_cpu() {
	// i want this script to simply alloc memory for the cpu
	// u16* sucess = malloc(*total_memory)
	// if (sucess != null) {
	//   printf("succesfully allocated \n");
	// } else {
	//   printf("failed to allocate main mem \n");
	// }
	// return 1;
	cpu_waiting = 1;
	printf("We did nothing lmao \n");
	return 0;
}

static inline int exec_instruction() {
	switch (instruction) {
		case ADD:
			printf("Enter the address to increment:");	
			scanf("%x", &address);

			printf("Old value: 0x%04x $%04x \n", total_memory[address], total_memory[address]);
			total_memory[address] += 1;
			printf("Current value: 0x%04x $%04x \n", total_memory[address], total_memory[address]);
		break;
		
		case MOV:
			printf("Enter the address and the destination:");
			scanf("%x %x", &address, &destination);

			printf("Old value: 0x%04x $%04x \n", total_memory[address], total_memory[address]);
			total_memory[address] = destination;
			printf("Current value: 0x%04x $%04x \n", total_memory[address], total_memory[address]);
		break;

		case PRN:
			printf("Select the start and the end address: ");
			scanf("%x %x", &address, &end);
			if (end < address) {
				printf("Error, end is smaller than start \n");
				break;
			}
			for(u16 i = address; i < end; i ++) {
				printf("HUMAN: %04x | SNES: $%04x \n", total_memory[i], total_memory[i]);
			}
		break;

		default:
			printf("Unknown instruction \n");
			cpu_waiting = 0;
		break;
	}
	return 0;
}


extern inline void mainFunc() {
	cpu_waiting = 1;
	while (cpu_waiting) {
		printf("Waiting for instruction... \n");
		printf("Enter the instruction: ");
		scanf("%hu", &instruction);
		
		if (address >= (sizeof(total_memory) / sizeof(total_memory[0]))) {
			printf("ERROR! Address exceed usable memory! \n");
			break;
		}
		exec_instruction();
		break;
	}
}

static inline void printMemory() {
	for(u16 i = 0; i < (sizeof(total_memory) / sizeof(total_memory[0])); i ++) {
		printf("Address number: %04x \n", total_memory[i]);
		sleep(1);
	}
}
