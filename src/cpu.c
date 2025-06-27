#include "defs.h"

#include "instructions.h"
#include "cpu_opcodes.h"
#include "arguments.h"
#include "memory.h"

#include <stdio.h>
#include <stdbool.h>
#include <unistd.h>
#include <string.h>

static int cpu_waiting;

static inline int exec_instruction() {
	//kinda hack but works;
	if (address >= (sizeof(total_memory) / sizeof(total_memory[0]))) {
			printf("ERROR! Address exceed usable memory! \n");
		}
	switch (instruction) {
		case _add:
			add();
		break;
		case _mov:
			mov();
		break;
		case _prn:
			prn();
		break;
		case _eoq:
			cpu_waiting = 0;
			exit(1);
		break;

		default:
			printf("Unknown instruction: %x %x \n", rom[pc], instruction);
			return 0;
		break;
	}
	return 0;
}

extern int mainFunc() {
	cpu_waiting = 1;
	while (cpu_waiting) {
		//printMemory();
		#ifdef DEBUG		
			printf("Waiting for instruction...");
			printf("Enter the instruction: ");
			scanf("%hx", &instruction);
		#else
			printf("Current instruction %x \n", rom[pc]); // this will just print the counter
			instruction = rom[pc++];
			exec_instruction();
			usleep(10000);
		#endif
		//break;
	}
	return 0;
}

static inline void printMemory() {
	for(int i = 0; i < (sizeof(total_memory) / sizeof(total_memory[0])); i ++) {
		printf("Address number: %04x \n", total_memory[i]);
	}
}
