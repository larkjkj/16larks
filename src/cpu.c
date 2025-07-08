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

i16 a = 0;
u16 s = 0;
i16 x, y = 0;

u16 dp, dbr = 0;
u16 pb, pbr = 0;

i8 z_flag, n_flag, v_flag, m_flag, x_flag, c_flag, d_flag, i_flag;

u16 instruction = 0;
u16 address = 0;
u16 destination = 0;
int cpu_waiting = 0;
char tot_mem_buffer[200];

//satic int cpu_waiting;
int cycle = 0;

static void execInstruction() {
	//kinda hack but works;	
	if (rom[pc] >= (sizeof(total_memory) / sizeof(total_memory[0]))) {
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
		case _inc_addr:
			inc_addr();
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

		case _clc:
			clc();
		break;

		case _cld:
			cld();
		break;

		case _cli:
			cli();
		break;

		case _clv:
			clv();
		break;
		case _cmp_addr:
			cmp_addr();
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

		case _inc_a:
			inc_a();
		break;

		case _inx:
			inx();
		break;

		case _iny:
			iny();
		break;

		case _sta_addr:
			sta_addr();
		break;

		case _stx_addr:
			stx_addr();
		break;

		case _sty_addr:
			sty_addr();
		break;
		case _tax:
			tax();
		break;
		
		case _tay:
			tay();
		break;
		
		case _tcd:
			tcd();
		break;
		
		case _tcs:
			tcs();
		break;

		case _tdc:
			tdc();
		break;

		case _tsc:
			tsc();
		break;

		case _tsx:
			tsx();
		break;
		
		case _txa:
			txa();
		break;

		case _txs:
			txs();
		break;

		case _txy:
			txy();
		break;

		case _tya:
			tya();
		break;

		case _tyx:
			tyx();
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
		for(int j = 0; j < dist; j += 1) {
			printf("0x%04X, ", total_memory[j]);
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
			printf("Current pointing at %x \n", rom[pc]); // this will just print the counter
		}

		instruction = rom[pc++];
		execInstruction();
		eventLoop();
		// this adda momentum for the instruction for exec (simulate cycles)
		//usleep(cycle);
		
		#endif
	}
	return 0;
}
