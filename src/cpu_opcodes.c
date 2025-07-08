#include <stdio.h>
#include <stdlib.h>

#include "rom.h"
#include "cpu.h"
#include "cpu_opcodes.h"
#include "memory.h"
#include "instructions.h"
#include "arguments.h"

#ifdef INTERACTIVE 
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

inline void clc() {
	c_flag = 0;
}

inline void cld() {
	d_flag = 0;
}

inline void cli() {
	i_flag = 0;
}

inline void clv() {
	v_flag = 0;
}

inline void cmp_addr() {
	address = rom[++pc];
	if (a = address) {
		z_flag = 1;
	} else {
		n_flag = 1;
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
	address = rom[++pc];
	rom[pc] = address;

	printf("%i \n", &rom[pc]);
	//address = rom[pc++];
}

inline void ato() {
	address = rom[++pc];
	end = rom[pc++];
	for(u16 i = address; i < end; i ++) {
		printf("%xc \n", &rom[i]);
	}
}
#endif
inline void inc_a() {
	a += 1;
}

inline void inc_addr() {
	address = rom[++pc];
	address += 1;
}

inline void inx() {
	x += 1;
}

inline void iny() {
	y += 1;
}

inline void tax() {
	x = a;
}

inline void tay() {
	y = a;
}

inline void tcd() {
	dp = a;
}

inline void tcs() {
	s = a;
}

inline void tdc() {
	a = dp;
}

inline void tsc() {
	a = s;
}

inline void tsx() {
	x = s;
}

inline void txa() {
	a = x;
}

inline void txs() {
	s = x;
}

inline void txy() {
	y = x;
}

inline void tya() {
	a = y;
}

inline void tyx() {
	x = y;
}

inline void sta_addr() {
	address = rom[++pc];
	total_memory[address] = rom[++pc];
}

inline void stx_addr() {
	address = rom[++pc];
	total_memory[address] = x;
}

inline void sty_addr() {
	address = rom[++pc];
	total_memory[address] = y;
}

