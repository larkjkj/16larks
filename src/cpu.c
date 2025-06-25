#include "defs.h"

#include "instructions.h"
#include "cpu_instructions.h"
#include "memory.h"

#include <stdio.h>
#include <stdbool.h>
#include <unistd.h>
#include <string.h>

bool cpu_waiting;

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
			add();
		break;
		case MOV:
			mov();
		break;
		case PRN:
			prn();
		break;

		default:
			printf("Unknown instruction \n");
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
	}
}

static inline void printMemory() {
	for(u16 i = 0; i < (sizeof(total_memory) / sizeof(total_memory[0])); i ++) {
		printf("Address number: %04x \n", total_memory[i]);
		sleep(1);
	}
}
