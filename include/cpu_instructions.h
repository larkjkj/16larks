#include <stdio.h>
#include <stdlib.h>

#include "arguments.h"
#include "cpu.h"
#include "memory.h"
#include "instructions.h"
#include "registers.h"

u16 destination;
u16 instruction;
u16 pc = 0; // program counter
int verbose = 0;

#ifndef DEBUG
// sequence of fake roms for testing
u16 fake_rom[] = {
	// MOV dados aleatórios em posições espalhadas
	_mov, 0x0012, 0x00FF,
	_mov, 0x00A0, 0x0042,
	_mov, 0x00C1, 0x0F0F,
	_mov, 0x0030, 0x1234,
	_mov, 0x0100, 0xAAAA,

	// ADD várias posições aleatórias
	_add, 0x0012,
	_add, 0x00A0,
	_add, 0x00A0,
	_add, 0x0030,
	_add, 0x0100,

	// PRN blocos específicos
	_prn, 0x0010, 0x0015,
	_prn, 0x00A0, 0x00A3,
	_prn, 0x00C0, 0x00C3,

	// MOV em sequência cruzada
	_mov, 0x0020, 0x0021,
	_mov, 0x0021, 0x0022,
	_mov, 0x0022, 0x0023,
	_mov, 0x0023, 0x0024,
	_mov, 0x0024, 0x0025,

	// ADD em looping de blocos
	_add, 0x0020,
	_add, 0x0021,
	_add, 0x0022,
	_add, 0x0023,
	_add, 0x0024,

	// PRN geral
	_prn, 0x0020, 0x0026,

	// MOV & ADD em área nova
	_mov, 0x00B5, 0x7777,
	_add, 0x00B5,
	_add, 0x00B5,

	// PRN em zona vazia
	_prn, 0x00B0, 0x00B8,

	// MOV dados altos
	_mov, 0x01F0, 0xDEAD,
	_mov, 0x01F1, 0xBEEF,
	_mov, 0x01F2, 0xC0DE,
	_add, 0x01F2,
	_add, 0x01F2,

	// PRN dados altos
	_prn, 0x01F0, 0x01F3,

	// Mistura final
	_mov, 0x0001, 0x9999,
	_add, 0x0001,
	_add, 0x0001,
	_prn, 0x0000, 0x0003,

	// MOV para testar sobrescrita
	_mov, 0x0030, 0xFFFF,
	_add, 0x0030,
	_add, 0x0030,
	_prn, 0x0030, 0x0031,

	// Encerramento
	_eoq
};
/*
u16 fake_rom[] = {
	// MOV 0x0011 → [0x0000]
	_mov, 0x0000, 0x0011,

	// MOV 0x000A → [0x0001]
	_mov, 0x0001, 0x000A,

	// MOV 0x00FF → [0x0002]
	_mov, 0x0002, 0x00FF,

	// ADD [0x0001] (valor 0x000A → vira 0x000B)
	_add, 0x0001,

	// ADD [0x0002] (0x00FF → 0x0100)
	_add, 0x0002,

	// PRN 0x0000 até 0x0003
	_prn, 0x0000, 0x0003,

	// MOV 0x1234 → [0x0010]
	_mov, 0x0010, 0x1234,

	// ADD [0x0010] → 0x1235
	_add, 0x0010,

	// PRN 0x0010 até 0x0012
	_prn, 0x0010, 0x0012,

	// Finaliza
	_eoq
};
/*u16 fake_rom[] = {
	// _mov = 0xfb
	// _add = 0xfa
	// _prn = 0xfc
	// _eoq = 0xfd
	_add, 0x0010,
	_mov, 0x0010, 0x0014,
	_prn, 0x0010, 0x0011,
	_eoq,

	_add, 0x0010,         // ADD 0x0010
	_mov, 0x0010, 0x0014,
	_prn, 0x0010, 0x0011,  // PRN 0x0010 → 0x0011
	_eoq                  // EOQ
};*/
#endif

#ifdef DEBUG
	static inline int add () {
		printf("Enter the address to increment:");	
		scanf("%x", &address);
		printf("Old value: 0x%04x $%04x \n", total_memory[address], total_memory[address]);
		total_memory[address] += 1;
		printf("Current value: 0x%04x $%04x \n", total_memory[address], total_memory[address]);		}

	static inline int mov () {
		printf("Enter the address and the destination:");
		scanf("%x %x", &address, &destination);
		printf("Old value: 0x%04x $%04x \n", total_memory[address], total_memory[address]);
		total_memory[address] = destination;
		printf("Current value: 0x%04x $%04x \n", total_memory[address], total_memory[address]);
	}

	static inline int prn () {
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
extern inline int add() {
	address = fake_rom[pc++];
	total_memory[address] += 1;
	if (verbose) {
		printf("Value is %x \n", total_memory[address]);
	}
	return address;
}

extern inline int mov() {
	address = fake_rom[pc++];
	destination = fake_rom[pc++];
	total_memory[address] = destination;
	if (verbose) {
		printf("%x \n", total_memory[address]);
	}
	return address;
}

extern inline int prn() {
	address	= fake_rom[pc++];
	end = fake_rom[pc++];
	if (verbose) {
		for(u16 i = address; i < end; i ++) {
			printf("HUMAN: 0x%04x | SNES: $%04x \n", total_memory[i], total_memory[i]);
		}
	}
}
#endif
