#include "defs.h"

#include "instructions.h"
#include "cpu_instructions.h"
#include "memory.h"

#include <stdio.h>
#include <stdbool.h>
#include <unistd.h>
#include <string.h>
int cpu_waiting = 1;

static inline int exec_instruction() {
	fake_rom[++pc];
	//kinda hack but works;
	#ifdef NOROM
		instruction = fake_rom[pc];
	#endif

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

		case _err:
			printf("Code is ignoring next value as argument \n");
		break;
		default:
			printf("Unknown instruction: %x \n", instruction);
			return 0;
		break;
	}
	return 0;
}

extern inline int mainFunc() {
	cpu_waiting = 1;
	while (cpu_waiting) {	
		#ifndef NOROM		
			printf("Waiting for instruction...");
			printf("Enter the instruction: ");
			scanf("%hx", &instruction);
		#endif
		printf("%x \n", fake_rom[pc]);

		if (address >= (sizeof(total_memory) / sizeof(total_memory[0]))) {
			printf("ERROR! Address exceed usable memory! \n");
			break;
		}
		exec_instruction();
		break;
	}
	return 0;
}

static inline void printMemory() {
	for(u16 i = 0; i < (sizeof(total_memory) / sizeof(total_memory[0])); i ++) {
		printf("Address number: %04x \n", total_memory[i]);
		sleep(1);
	}
}
