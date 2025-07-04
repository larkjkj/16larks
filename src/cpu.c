#include "defs.h"

#include "instructions.h"
#include "cpu_opcodes.h"
#include "cpu.h"
#include "init_funcs.h"

#include "arguments.h"
#include "memory.h"
#include "rom.h"

#include <stdio.h>
#include <stdbool.h>
#include <unistd.h>
#include <string.h>

u16 total_memory[65536] = {};
u16 pc = 0;
u16 instruction = 0;
u16 address = 0;
u16 destination = 0;
int cpu_waiting = 0;
char tot_mem_buffer[200];

//satic int cpu_waiting;
int cycle = 0;

static void execInstruction() {
	//kinda hack but works;	
	if (instruction >= (sizeof(total_memory) / sizeof(total_memory[0]))) {
		printf("ERROR! Address exceed usable memory! \n");
	}
	switch (instruction) {
		//emulator ones
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
		case _ato:
			ato();
		break;


	
		// actual snes
		case _inc:
			add();
		break;
		case _asl_dp:
			printf("ASL DP \n");
		break;
		case _asl_a:
			printf("ASL A \n");
	break;
		case _asl_addr:
			printf("ASL ADDR \n");
		break;
		case _asl_dp_x:
			printf("ASL DP X \n");
		break;
		case _asl_addr_x:
			printf("ASL ADDR X \n");
		break;
		case _and_sr_s:
			printf("AND SR S \n");
		break;
		case _bit_dp:
			printf("BIT DP \n");
		break;
		case _bit_addr:
			printf("BIT ADDR \n");
		break;
		case _bit_dp_x:
			printf("BIT DP X \n");
		break;
		case _bit_addr_x:
			printf("BIT ADDR X \n");
		break;
		case _bit_const:
			printf("BIT CONST \n");
		break;
		/*
		case _and_sr_s:
			printf("AND SR S");
		break;
		case _and_sr_s:
			printf("AND SR S");
		break;
		case _and_sr_s:
			printf("AND SR S");
		*/
		break;
		/*case _jmp: //bye old friend :(
			jmp();
			printf("JMP");
		break;*/
		case _jmp_addr:
			jmp();
			printf("JMP ADDR \n");
		break;
		case _jmp_long:
			printf("JMP LONG \n");
		break;
		case _jmp_addr_x:
			printf("JMP ADDR X \n");
		break;
		case _jmp_addr_long:
			printf("JMP ADDR LONG \n");
		break;
		case _jmp_addr_abs:
			printf("JMP ADDR ABS \n");
		break;
		/*
		case _and_sr_s:
			printf("AND SR S");
		break;
		case _and_sr_s:
			printf("AND SR S");
		break;
		case _and_sr_s:
			printf("AND SR S");
		break;
		case _and_sr_s:
			printf("AND SR S");
		break;
*/
		case _and_dp_indr_l:
			printf("AND DP INDIRECT LONG \n");
		break;

		case _and_dp:
			printf("AND DP \n");
		break;

		case _and_const:
			printf("AND CONST \n");
		break;

		default:
			printf("Unknown instruction: %04X \n", instruction);
		break;
	}
}

extern void printMemory() {
	const int dist = 4;
	const int rows = 4;


	for(int i = 0; i < rows; i += 1) {
		printf("ROW NUMBER %i \n", i);
		for(int j = 1; j < dist; j += 1) {
			printf("0x%04X, ", total_memory[dist + j]);
			snprintf(tot_mem_buffer, sizeof(tot_mem_buffer), "0x%04X", total_memory[dist+j]);
		}
		printf("\n");
	}
}
extern int mainFunc() {
	cpu_waiting = 1;
	while (cpu_waiting) {
		//printMemory();
		#ifdef INTERACTIVE		
		printf("Waiting for instruction...");
		printf("Enter the instruction: ");
		scanf("%hx", &instruction);
		#else
		
		if (verbose) {
			printf("Current instruction %x \n", rom[pc]); // this will just print the counter
		}

		instruction = rom[++pc];
		execInstruction();
		eventLoop();
		// this adda momentum for the instruction for exec (simulate cycles)
		//usleep(cycle);

		#endif
		//break;
	}
	return 0;
}
