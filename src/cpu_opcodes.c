#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "rom.h"
#include "cpu.h"
#include "cpu_opcodes.h"
#include "memory.h"
#include "instructions.h"
#include "arguments.h"

/* WARNING, there's no difference between
 * address and offset AT ALL, they're basically
 * used as placeholders, and the names are used 
 * like that 'cause naming things is hard :( */

/* Interactive is disabled for now, it kinda broke(expected) when i added new opcodes
 * So in a future commit it'll added again :) */


/*
#ifdef INTERACTIVE 
inline void add() {
	printf("Enter the address to increment:");	
	scanf("%x", &address);
	printf("Old value: 0x%04x $%04x \n", total_memory[address], total_memory[address]);
	total_memory[address] += 1;
	printf("Current value: 0x%04x $%04x \n", total_memory[address], total_memory[address]);		}

inline void jmp() {
	printf("Select the address \n");
	scanf("%x", &address);
	address = rom[++pc];

	printf("%i \n", &rom[pc]);
	//address = rom[pc++];
}

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

inline void mov() {
	address = rom[pc++];
	destination = rom[pc++];
	total_memory[address] = destination;
	if (verbose) {
		printf("%x \n", total_memory[address]);
	}
	//return address;
}

inline void ato() {
	address = rom[++pc];
	end = rom[pc++];
	for(u16 i = address; i < end; i ++) {
		printf("%xc \n", &rom[i]);
	}
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
#endif
*/

inline void adc_dp_x_indr() {
	address = rom[++pc];
	
	byte_low = (total_memory[address + dp + x]);
	byte_high = (total_memory[address + dp + x + 1]);

	address = (byte_high << 8 | byte_low);
	// This shouldn't be a problem since
	// C Flag only contains 1 and 0.

	c_flag = ((a & compare_hex) ? 0 : 1);
	a += total_memory[address] + c_flag;
	flags_arr[0] = a;
	update_n_flag(1, flags_arr);
	update_z_flag(1, flags_arr);
}

inline void adc_dp_indr_l() {
	address = rom[++pc] + dp;
	byte_low = (total_memory[address] + 1);
	byte_high = (total_memory[address + 2]);

	address = (byte_high << 16 | ((byte_low << 8) | address));

	c_flag = ((a & compare_hex) ? 0 : 1);

	a += total_memory[address] + c_flag;
	flags_arr[0] = a;
	update_n_flag(1, flags_arr);
	update_z_flag(1, flags_arr);
}

inline void adc_sr_s() {
	address = rom[++pc];

	a += ((i8) stack[s + address] + c_flag);
}

inline void adc_dp() {
	address = rom[++pc] + dp;
	compare_hex = 0xFF;
	// This shouldn't be a problem since
	// C Flag only contains 1 and 0.

	c_flag = ((a & compare_hex) ? 0 : 1);

	a += total_memory[address] + c_flag;
	flags_arr[0] = a;
	update_n_flag(1, flags_arr);
	update_z_flag(1, flags_arr);
}

inline void adc_dp_indr() {
	offset = rom[++pc];
	compare_hex = 0xFFFF;	
	byte_low = (total_memory[offset + dp]);
	byte_high = (total_memory[offset + dp + 1]);

	address = (byte_high << 8 | byte_low);
	// This shouldn't be a problem since
	// C Flag only contains 1 and 0.
	
	c_flag = ((a & compare_hex) ? 0 : 1);

	a += total_memory[address] + c_flag;

	flags_arr[0] = a;
	update_n_flag(1, flags_arr);
	update_z_flag(1, flags_arr);
}


inline void adc_const() {
	byte_low = rom[++pc];
	byte_high = rom[++pc];
	
	if (!m_flag) {
		address = (byte_high << 8 | byte_low);
		compare_hex = 0xFFFF;
	} else {
		address = rom[++pc];
		compare_hex = 0xFF;
	}

	c_flag = ((a & compare_hex) ? 0 : 1);
	a += address + c_flag;
	flags_arr[0] = a;

	update_n_flag(1, flags_arr);
	update_z_flag(1, flags_arr);
}

inline void adc_addr() {
	byte_low = rom[++pc];
	byte_high = rom[++pc];
	compare_hex = 0xFFFF;
	address = (byte_high << 8 | byte_low);

	c_flag = ((a & compare_hex) ? 0 : 1);
	a += total_memory[address] + c_flag;
	flags_arr[0] = a;

	update_n_flag(1, flags_arr);
	update_z_flag(1, flags_arr);
}

inline void adc_l() {
	address = rom[++pc];
	byte_low = rom[++pc];
	byte_high = rom[++pc];
	address |= (byte_high << 16 | byte_low << 8 & address);

	a += total_memory[address] + c_flag;
	flags_arr[0] = a;

	update_n_flag(1, flags_arr);
	update_z_flag(1, flags_arr);
}

inline void adc_dp_y() {
	address = rom[++pc] + dp + y;
	compare_hex = 0xFF;
	a += total_memory[address] + c_flag;
		
	c_flag = ((a & compare_hex) ? 0 : 1);
	flags_arr[0] = a;

	update_n_flag(1, flags_arr);
	update_z_flag(1, flags_arr);
}

inline void adc_sr_s_indr_y() {
	byte_low = stack[++s];
	byte_high = stack[++s];

	address = (byte_high << 8 | byte_low) + y;
	c_flag = ((a & compare_hex) ? 0 : 1);
	a += total_memory[address] + c_flag;

	flags_arr[0] = a;

	update_n_flag(1, flags_arr);
	update_z_flag(1, flags_arr);
}

inline void adc_dp_x() {
	address = rom[++pc] + dp + x;
	flags_arr[0] = a;

	c_flag = ((a & compare_hex) ? 0 : 1);
	a += total_memory[address] + c_flag;

	update_n_flag(1, flags_arr);
	update_z_flag(1, flags_arr);
}

inline void adc_addr_y() {
	byte_low = rom[++pc];
	byte_high = rom[++pc];
	
	address = (byte_high << 8 | byte_low) + x;
	c_flag = ((a & compare_hex) ? 0 : 1);
	a += total_memory[address] + c_flag;

	flags_arr[0] = a;

	update_n_flag(1, flags_arr);
	update_z_flag(1, flags_arr);
}

inline void adc_addr_x() {
	byte_low = rom[++pc];
	byte_high = rom[++pc];
	
	address = (byte_high << 8 | byte_low) + x;

	c_flag = ((a & compare_hex) ? 0 : 1);
	a += total_memory[address] + c_flag;
	flags_arr[0] = a;

	update_n_flag(1, flags_arr);
	update_z_flag(1, flags_arr);
}

inline void adc_long_x() {
	byte_low = rom[++pc];
	byte_high = rom[++pc];

	address = (byte_high << 16 | byte_low << 8 | rom[++pc]) + x;
	a += total_memory[address] + c_flag;
	flags_arr[0] = a;

	update_n_flag(1, flags_arr);
	update_z_flag(1, flags_arr);
}

inline void and_sr_s_indr_y() {
	byte_low = stack[s];
	byte_high = stack[s++];

	address = (byte_high << 8 | byte_low) + y;

	operation = (a & total_memory[address]);

	flags_arr[0] = operation;
	update_n_flag(1, flags_arr);
	update_z_flag(1, flags_arr);
}

inline void and_addr() {
	byte_low = rom[++pc]; 
	byte_high = rom[++pc];

	address = (byte_high << 8 | byte_low);

	a = (total_memory[address] & a);
	flags_arr[0] = a;

	update_n_flag(1, flags_arr);
	update_z_flag(1, flags_arr);
}

inline void and_dp() {
	address = rom[++pc] + dp;

	a &= total_memory[address];
	flags_arr[0] = a;

	update_n_flag(1, flags_arr);
	update_z_flag(1, flags_arr);
}

inline void and_const() {
	byte_low = rom[++pc];
	byte_high = rom[++pc];

	if (!m_flag)
		address = (byte_high << 16 | byte_low << 8 | address);
	else
		address = (byte_low);

	a &= (address);
	flags_arr[0] = a;

	update_n_flag(1, flags_arr);
	update_z_flag(1, flags_arr);
}

inline void and_dp_x_indr() {
	offset = rom[++pc];
	address = (offset + dp + x);
	byte_low = (address + 1);

	address = ((total_memory[byte_low] << 8 | total_memory[address]) & 0xFFFF);
	flags_arr[0] = a;
	a &= (total_memory[address]);

	update_n_flag(1, flags_arr);
	update_z_flag(1, flags_arr);
	return;
}

inline void and_dp_x() {
	address = rom[++pc] + dp + x;
	a &= (address);
	flags_arr[0] = a;

	update_n_flag(1, flags_arr);
	update_z_flag(1, flags_arr);
}


inline void and_addr_x() {
	byte_low = rom[++pc];
	byte_high = rom[++pc];

	address = (byte_high << 8 | byte_low) + x;
	a = (total_memory[address] & a);
	flags_arr[0] = a;

	update_n_flag(1, flags_arr);
	update_z_flag(1, flags_arr);
}

inline void and_addr_y() {
	byte_low = rom[++pc];
	byte_high = rom[++pc];

	address = (byte_high << 8 | byte_low) + y;
	a = (total_memory[address] & a);

	flags_arr[0] = a;
	update_n_flag(1, flags_arr);
	update_z_flag(1, flags_arr);
}

inline void and_sr_s() {
	byte_low = stack[s];
	byte_high = stack[s++];

	address = (byte_high << 8 | byte_low);

	operation = (a & total_memory[address]);

	flags_arr[0] = operation;
	update_n_flag(1, flags_arr);
	update_z_flag(1, flags_arr);
}

inline void asl_dp() {
	address = dp;
	total_memory[address] <<= 1;
	
	flags_arr[0] = total_memory[address]; 

	update_n_flag(1, flags_arr);
	update_z_flag(1, flags_arr);
}

inline void asl_addr() {
	byte_low = rom[++pc];
	byte_high = rom[++pc];

	address = (byte_high << 8 | byte_low);
	total_memory[address] <<= 1;

	flags_arr[0] = total_memory[address];

	update_n_flag(1, flags_arr);
	update_z_flag(1, flags_arr);
}

inline void asl_addr_x() {
	byte_low = rom[++pc];
	byte_high = rom[++pc];

	address = (byte_high << 8 | byte_low) + x;
	total_memory[address] <<= 1;
	
	flags_arr[0] = total_memory[address];

	update_n_flag(1, flags_arr);
	update_z_flag(1, flags_arr);
}

inline void asl_dp_x() {
	address = rom[++pc] + dp + x;
	total_memory[address] <<= 1;
	
	flags_arr[0] = total_memory[address];

	update_n_flag(1, flags_arr);
	update_z_flag(1, flags_arr);
}

inline void asl_a() {
	a <<= 1;

	flags_arr[0] = a;

	update_n_flag(1, flags_arr);
	update_z_flag(1, flags_arr);
	//switch statement depending of the cpu mode 
	//for now we are using 16 bits so...
}

inline void bcc() {
	address = rom[pc++];
	if (!c_flag) {
		pc = (pc + (i8) address);
	}
}

inline void bcs() {
	address = rom[pc++];
	if (c_flag) {
		address = (pc + (i8) address);
		pc = address;
	}
}

inline void beq() {
	address = rom[pc++];
	if (z_flag) {
		pc = (pc + (i8) address);
	}
}

inline void bit_addr() {
	byte_low = rom[++pc];
	byte_high = rom[++pc];

	address = (byte_high << 8 | byte_low);
	
	operation = (total_memory[address] & a);

	flags_arr[0] = operation;
	update_n_flag(1, flags_arr);
	update_z_flag(1, flags_arr);
};

inline void bit_dp_x() {
	/* This code is wrong as shit
	address = rom[++pc];
	flags_arr[0] = a;
	flags_arr[1] = total_memory[address + dp + x];

	update_z_flag(2, flags_arr);
	update_n_flag(1, &flags_arr[1]);*/

	address = rom[++pc];
	operation = (total_memory[address + dp + x] & a);

	flags_arr[0] = operation;

	update_n_flag(1, flags_arr);
	update_z_flag(1, flags_arr);
}

inline void bit_addr_x() {
	byte_low = rom[++pc];
	byte_high = rom[++pc];

	address = (byte_high << 8 | byte_low) + x;	

	operation = (total_memory[address] & a);
	flags_arr[0] = operation;

	update_n_flag(1, flags_arr);
	update_z_flag(1, flags_arr);
};

inline void bit_dp() {
	address = rom[++pc] + dp;

	operation = (total_memory[address] & a);
	
	flags_arr[0] = operation;
	update_n_flag(1, flags_arr);
	update_z_flag(1, flags_arr);
}

inline void bit_const() {
	byte_low = rom[++pc];
	byte_high = rom[++pc];
	
	if (!m_flag)
		address = (byte_high << 8 | byte_low);
	else
		address = byte_low;

	flags_arr[0] = (address & a);

	update_n_flag(1, flags_arr);
	update_z_flag(1, flags_arr);
}

inline void brk_emu() {
	if (!m_flag)
		pc = rom[(0x00ffe6 & 0xffffff)];
	else
		pc = rom[(0x00fff6 & 0xffffff)];
	
	i_flag = 1;
}

/* Maybe i can mix these functions and just make a switch
 * for an Optimized-Interpreter-Mode */
inline void bmi() {
	if (n_flag) {
		address = rom[++pc];
		pc = (i8) pc + address;
	}
}

inline void bne() {
	if (!z_flag) {
		address = rom[++pc];
		pc = (i8) pc + address;
	}
}

inline void bpl() {
	if (n_flag) {
		address = rom[++pc];
		pc = (i8) pc + address;
	}
}

inline void bra() {
	address = ((i8) rom[++pc]);
	pc = pc + address;
}

inline void brl() {
	byte_low = rom[++pc];
	byte_high = rom[++pc];

	address &= (byte_high << 8 | byte_low) & 0xFFFF;
	pc = (pc + (i16) address);
}

inline void bvc() {
	address = rom[++pc];

	if (!v_flag) {
		pc += (i8) address;
	}
}

inline void bvs() {
	address = rom[++pc];
	
	if (v_flag) {
		pc += (i8) address;
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

inline void cmp_dp_x_indr() {
	address = rom[++pc] + dp + x;

	byte_low = (total_memory[address]);
	byte_high = (total_memory[address + 1]);

	address = (byte_high << 8 | byte_low);
	operation = (a - total_memory[address]);
	
	flags_arr[0] = operation;
	update_n_flag(1, flags_arr);
	update_z_flag(1, flags_arr);
}

inline void cmp_sr_s() {
	operation = (a - (i8) stack[s]);

	flags_arr[0] = operation;
	update_n_flag(1, flags_arr);
	update_z_flag(1, flags_arr);
}

inline void cmp_const() {
	byte_low = rom[++pc];
	byte_high = rom[++pc];

	operation = (m_flag ? (a - (byte_high << 8) | byte_low) : (a - rom[++pc]));

	flags_arr[0] = operation;

	update_n_flag(1, flags_arr);
	update_z_flag(1, flags_arr);
}

inline void cmp_addr() {
	byte_low = rom[++pc];
	byte_high = rom[++pc];
	
	address = (byte_high << 8 | byte_low);
	flags_arr[0] = a - total_memory[address];

	update_n_flag(1, flags_arr);
	update_z_flag(1, flags_arr);
}

inline void cmp_l() {
	byte_low = rom[++pc];
	byte_high = rom[++pc];

	address = (rom[++pc] << 16 | byte_high << 8 | byte_low);

	flags_arr[0] = total_memory[address] - a;

	update_n_flag(1, flags_arr);
	update_z_flag(1, flags_arr);
};

inline void cmp_l_x() {
	byte_low = rom[++pc];
	byte_high = rom[++pc];

	address = (byte_high << 16 | byte_low << 8 | rom[++pc]) + x;

	flags_arr[0] = total_memory[address] - a;

	update_n_flag(1, flags_arr);
	update_z_flag(1, flags_arr);
};

inline void cmp_dp_indr_y() {
	address = rom[++pc] + dp;
	byte_low = (total_memory[address]);
	byte_high = (total_memory[address + 1]);

	address = (byte_high << 8 | byte_low) + y;
	operation = (a - total_memory[address]);

	flags_arr[0] = operation;

	update_n_flag(1, flags_arr);
	update_z_flag(1, flags_arr);
}

inline void cmp_dp() {
	address = rom[++pc] + dp;

	flags_arr[0] = a - total_memory[address];

	update_n_flag(1, flags_arr);
	update_z_flag(1, flags_arr);
}

inline void cmp_dp_indr() {
	address = rom[++pc] + dp;

	byte_low = (total_memory[address]);
	byte_high = (total_memory[address + 1]);

	address = (byte_high << 8 | byte_low);
	operation = (a - total_memory[address]);
	flags_arr[0] = operation;

	update_n_flag(1, flags_arr);
	update_z_flag(1, flags_arr);
}

inline void cmp_dp_x() {
	address = rom[++pc] + dp + x;

	operation = (a - total_memory[address]);

	flags_arr[0] = operation;

	update_n_flag(1, flags_arr);
	update_z_flag(1, flags_arr);

}

inline void cmp_addr_y() {
	byte_low = rom[++pc];
	byte_high = rom[++pc];
	
	address = (byte_high << 8 | byte_low) + y;
	operation = (a - total_memory[address]);
	flags_arr[0] = operation;

	update_n_flag(1, flags_arr);
	update_z_flag(1, flags_arr);
	//update_c_flag(1, flags_arr);
}

inline void cmp_addr_x() {
	byte_low = rom[++pc];
	byte_high = rom[++pc];
	
	address = (byte_high << 8 | byte_low) + x;
	operation = (a - total_memory[address]);
	flags_arr[0] = operation;

	update_n_flag(1, flags_arr);
	update_z_flag(1, flags_arr);
}

inline void cop_const() {
	printf("Not implemented COP Const \n");
	if (!m_flag) {
		
	} else {

	}
/*
	update_n_flag(1, flags_arr);
	update_z_flag(1, flags_arr);*/
}

inline void cpx_addr() {
	byte_low = rom[++pc];
	byte_high = rom[++pc];
	
	address = (byte_high << 8 | byte_low);

	flags_arr[0] = x - total_memory[address];

	update_n_flag(1, flags_arr);
	update_z_flag(1, flags_arr);
	/*
	n_flag = ((total_memory[address] & a) ? 1 : 0);
	z_flag = ((total_memory[address] & a == 0) | (total_memory[address] == a) ? 1 : 0);*/
}

inline void cpx_dp() {
	byte_low = rom[++pc] + dp;
	byte_high = rom[++pc] + dp;
	
	address = (byte_high << 8 | byte_low);

	flags_arr[0] = x - total_memory[address];

	update_n_flag(1, flags_arr);
	update_z_flag(1, flags_arr);
	/*
	n_flag = ((total_memory[address] & a) ? 1 : 0);
	z_flag = ((total_memory[address] & a == 0) | (total_memory[address] == a) ? 1 : 0);*/
}
inline void cpx_const() {
	if (x_flag == 0) {
		byte_low = rom[++pc];
		byte_high = rom[++pc];

		address = (byte_high << 8 | byte_low);
	} else {
		address = rom[++pc];
	}

	flags_arr[0] = x - address;//stuff;

	update_n_flag(1, flags_arr);
	update_n_flag(1, flags_arr);
}


inline void cpy_const() {
	if (x_flag == 0) {
		byte_low = rom[++pc];
		byte_high = rom[++pc];

		address = (byte_high << 8 | byte_low);
	} else {
		address = rom[++pc];
	}

	flags_arr[0] = y - address;//stuff;

	update_n_flag(1, flags_arr);
	update_n_flag(1, flags_arr);
}

inline void cpy_addr() {
	byte_low = rom[++pc];
	byte_high = rom[++pc];
	
	address = (byte_high << 8 | byte_low);

	flags_arr[0] = y - total_memory[address];

	update_n_flag(1, flags_arr);
	update_z_flag(1, flags_arr);
}

inline void cpy_dp() {
	byte_low = rom[++pc] + dp;
	byte_high = rom[++pc] + dp;
	
	address = (byte_high << 8 | byte_low);

	flags_arr[0] = y - total_memory[address];

	update_n_flag(1, flags_arr);
	update_z_flag(1, flags_arr);
}


inline void dec_a() {
	a -= 1;

	flags_arr[0] = a;
	update_n_flag(1, flags_arr);
	update_z_flag(1, flags_arr);
}

inline void dec_dp() {
	address = rom[++pc] + dp;
	total_memory[address] -= 1;

	flags_arr[0] = total_memory[address];
	update_n_flag(1, flags_arr);
	update_z_flag(1, flags_arr);
}

inline void dec_addr() {
	address = rom[++pc];
	total_memory[address] -= 1;

	flags_arr[0] = total_memory[address];
	update_n_flag(1, flags_arr);
	update_z_flag(1, flags_arr);
}

inline void dec_dp_x() {
	address = rom[++pc] + dp + x;
	
	total_memory[address] -= 1;

	flags_arr[0] = total_memory[address];
	update_n_flag(1, flags_arr);
	update_z_flag(1, flags_arr);
}

inline void dec_addr_x() {
	address = rom[pc] + x;

	total_memory[address] -= 1;

	flags_arr[0] = total_memory[address];

	update_n_flag(1, flags_arr);
	update_z_flag(1, flags_arr);
}

inline void dex_implied() {
	x -= 1;
}

inline void dey_implied() {
	y -= 1;
}

inline void eor_dp_x_indr() {
	address = rom[++pc] + dp + x;

	byte_low = total_memory[address];
	byte_high = total_memory[address + 1];

	address = (byte_high << 8 | byte_low);
	operation = (a ^ total_memory[address]);

	flags_arr[0] = operation;

	update_n_flag(1, flags_arr);
	update_z_flag(1, flags_arr);
	return;
} 

inline void eor_sr_s() {
	// yes this is different, not a name misleading
	operation = (a ^ (i8) stack[s]);
	flags_arr[0] = operation;

	update_n_flag(1, flags_arr);
	update_z_flag(1, flags_arr);

}

inline void eor_sr_s_indr_y() {
	printf("Not implemented EOR SR S Indr Y \n");
}

inline void eor_dp() {
	address = rom[++pc] + dp;

	operation = (a ^ total_memory[address]);
	flags_arr[0] = operation;

	update_n_flag(1, flags_arr);
	update_z_flag(1, flags_arr);
}

inline void eor_dp_x() {
	address = rom[++pc] + dp + x;

	operation = (a ^ total_memory[address]);
	flags_arr[0] = operation;

	update_n_flag(1, flags_arr);
	update_z_flag(1, flags_arr);
}

inline void eor_dp_indr_y() {
	address = rom[++pc] + dp + y;
	byte_low = total_memory[address];
	byte_high = total_memory[address + 1];

	address = (byte_high << 8 | byte_low);
	operation = (a ^ total_memory[address]);

	flags_arr[0] = operation;
	update_n_flag(1, flags_arr);
	update_z_flag(1, flags_arr);
}

inline void eor_dp_indr() {
	address = rom[++pc] + dp;
	byte_low = total_memory[address];
	byte_high = total_memory[address + 1];

	address = (byte_high << 8 | byte_low);
	operation = (a ^ total_memory[address]);

	flags_arr[0] = operation;
	update_n_flag(1, flags_arr);
	update_z_flag(1, flags_arr);
}

inline void eor_const() {
	if (m_flag) {
		address = rom[++pc];
	} else {
		byte_low = rom[++pc];
		byte_high = rom[++pc];

		address = (byte_high << 8 | byte_low);
	}

	operation = (a ^ address);

	flags_arr[0] = operation;

	update_n_flag(1, flags_arr);
	update_z_flag(1, flags_arr);
}

inline void eor_addr() {
	byte_low = rom[++pc];
	byte_high = rom[++pc];

	address = (byte_high << 8 | byte_low);

	operation = (a ^ total_memory[address]);
	flags_arr[0] = operation;

	update_n_flag(1, flags_arr);
	update_z_flag(1, flags_arr);
}

inline void eor_addr_y() {
	byte_low = rom[++pc];
	byte_high = rom[++pc];

	address = (byte_high << 8 | byte_low) + y;

	operation = (a ^ total_memory[address]);
	flags_arr[0] = operation;

	update_n_flag(1, flags_arr);
	update_z_flag(1, flags_arr);
}

inline void eor_dp_indr_x() {
	address = rom[++pc] + x;
	
	byte_low = (total_memory[address + dp]);
	byte_high = (total_memory[address + dp + 1]);

	address = (byte_high << 8 | byte_low);
	a = (a | total_memory[address]);

	flags_arr[0] = a;
	update_n_flag(1, flags_arr);
	update_z_flag(1, flags_arr);
}



inline void eor_addr_x() {
	byte_low = rom[++pc];
	byte_high = rom[++pc];

	address = (byte_high << 8 | byte_low) + x;

	operation = (a ^ total_memory[address]);
	flags_arr[0] = operation;

	update_n_flag(1, flags_arr);
	update_z_flag(1, flags_arr);
}

inline void mvn() {
	//The inverse of MVP
	byte_low = rom[++pc];
	byte_high = rom[++pc];

	u16 x_increment = (x + a);
	u16 y_increment = (y + a);

	for(int i = 0; i < (x - a); i ++) {
		total_memory[byte_high + i] = total_memory[byte_low + i];
		x ++;
		y ++;
		a --;
		if (a & 0xFFFF) {
			printf("Accumulator exceeds negative \n");
			break;
		}
	}
}

inline void mvp() {
	//This is an hard opcode to explain but basically, The following addresses are indexs
	byte_low = rom[++pc];
	byte_high = rom[++pc];

	u16 x_increment = (x - a);
	u16 y_increment = (y - a);

	for(int i = 0; i < (x - a); i ++) {
		total_memory[byte_high + i] = total_memory[byte_low + i];
		x --;
		y --;
		a --;
		if (a & 0xFFFF) {
			printf("Accumulator exceeds negative \n");
			break;
		}
	}/*
	for(int i = total_memory[x]; i < total_memory[y]; i ++) {
		for(int j = x_increment; j < y_increment; j ++) {
			total_memory[i] = j;
			x --;
			y --;
			a --;
			if (a & 0xFF) {
				printf("Accumulator exceeds negative \n");
				break;
			}
		}
	}
	// nutes to be copied [X startpoint:] (x_increment) to [Y startpoint] (y_increment)
	/*
	for(int i = 0; i < x_increment; i ++) {
		for(int j = 0; j < y_increment; j ++) {
			total_memory[x++] = j
		
		}
	}*/

}

inline void nop() {
	return;
}

inline void inc_a() {
	a += 1;

	flags_arr[0] = a;

	update_n_flag(1, flags_arr);
	update_z_flag(1, flags_arr);
}

inline void inc_addr() {
	address = rom[++pc];
	total_memory[address] += 1;

	flags_arr[0] = total_memory[address];

	update_n_flag(1, flags_arr);
	update_z_flag(1, flags_arr);

}

inline void inc_addr_x() {
	address = rom[++pc] + x;
	total_memory[address] += 1;

	flags_arr[0] = total_memory[address];

	update_n_flag(1, flags_arr);
	update_z_flag(1, flags_arr);

}

inline void inc_dp() {
	address = rom[++pc] + dp;
	total_memory[address] += 1;

	flags_arr[0] = total_memory[address];

	update_n_flag(1, flags_arr);
	update_z_flag(1, flags_arr);

}

inline void inc_dp_x() {
	address = rom[++pc] + dp + x;
	total_memory[address] += 1;

	flags_arr[0] = total_memory[address];

	update_n_flag(1, flags_arr);
	update_z_flag(1, flags_arr);
}

inline void inx() {
	x += 1;

	flags_arr[0] = x;

	update_n_flag(1, flags_arr);
	update_z_flag(1, flags_arr);
}

inline void iny() {
	y += 1;

	flags_arr[0] = y;

	update_n_flag(1, flags_arr);
	update_z_flag(1, flags_arr);
}

inline void jmp_addr() {
	byte_low = rom[++pc];
	byte_high = rom[++pc];

	address = (byte_high << 8 | byte_low);
	rom[pc] = address;
}

inline void jmp_addr_indr() {
	byte_low = rom[++pc];
	byte_high = rom[++pc];

	address = (byte_high << 8 | byte_low);

	address = (total_memory[address] << 8 | total_memory[address + 1]);
	rom[pc] = address;
}

inline void jmp_addr_x_indr() {
	byte_low = rom[++pc];
	byte_high = rom[++pc];

	address = (byte_high << 8 | byte_low) + x;

	address = (total_memory[address] << 8 | total_memory[address + 1]);
	rom[pc] = address;
}

inline void jmp_addr_l() {
	address = rom[++pc];
	byte_low = rom[++pc];
	byte_high = rom[++pc];

	address = ((byte_high << 16) | byte_low << 8 | address);

	rom[pc] = address;
}

inline void jmp_addr_indr_l() {
	address = rom[++pc];

	byte_low = (total_memory[address + 1]);
	byte_high = (total_memory[address + 2]);

	address = ((byte_high << 16 | byte_low << 8 | total_memory[address]));

	rom[pc] = total_memory[address];
	return;
}

inline void jsr_addr() {
	byte_low = rom[++pc];
	byte_high = rom[++pc];

	address = (byte_high << 8 | byte_low);
	saved_pc = address + 1; // This is the same thing, but a custom variable
	stack[s--] = (address + 1); //Since stack is emulated as 16-bit we can simply
				    //assign it to stack without making a byte-to-byte

	pc = address;
}

inline void jsr_long() {
	address = rom[++pc];
	byte_low = rom[++pc];
	byte_high = rom[++pc];

	address = (byte_high << 16 | byte_low << 8 | address);

	stack[s--] = (address + 1);

	pc = address;
}

inline void jsr_addr_x_indr() {
	printf("Not implemented.. yet \n");
	return;
}

inline void lda_addr() {
	address = rom[++pc];
	byte_low = rom[++pc];
	byte_high = rom[++pc];

	address = (byte_high << 16 | byte_low << 8 | address);
	a = total_memory[address];

	flags_arr[0] = a;

	update_n_flag(1, flags_arr);
	update_z_flag(1, flags_arr);
}

inline void lda_const() {
	address = rom[++pc];
	byte_low = rom[++pc];
	byte_high = rom[++pc];

	if (!m_flag) {
		address = ((byte_high << 16 | byte_low << 8 | address));
	} else {
		address = ((byte_low << 8 | address));
	}
	a = address;
	flags_arr[0] = a;

	update_n_flag(1, flags_arr);
	update_z_flag(1, flags_arr);
}


inline void lda_dp() {
	address = rom[++pc] + dp;
	a = total_memory[address];	
	
	flags_arr[0] = a;

	update_n_flag(1, flags_arr);
	update_z_flag(1, flags_arr);
}

inline void lda_dp_indr_l_y() {
	address = rom[++pc] + dp;

	byte_low = (total_memory[address + 1]);
	byte_high = (total_memory[address + 2]);

	address = (byte_high << 16 | byte_low << 8 | total_memory[address]);

	a = total_memory[address + y];

	flags_arr[0] = a;

	update_n_flag(1, flags_arr);
	update_z_flag(1, flags_arr);
}

inline void lda_dp_indr_l() {
	address = rom[++pc] + dp;

	byte_low = (total_memory[address + 1]);
	byte_high = (total_memory[address + 2]);

	address = (byte_high << 16 | byte_low << 8 | total_memory[address]);

	a = total_memory[address];

	flags_arr[0] = a;

	update_n_flag(1, flags_arr);
	update_z_flag(1, flags_arr);
}

inline void lda_dp_indr_y() {
	address = rom[++pc] + dp;

	byte_low = (total_memory[address]);
	byte_high = (total_memory[address + 1]);

	address = (byte_high << 8 | byte_low);

	a = total_memory[address + y];

	flags_arr[0] = a;

	update_n_flag(1, flags_arr);
	update_z_flag(1, flags_arr);
}


inline void lda_dp_x() {
	address = rom[++pc] + dp + x;
	a = total_memory[address];
	
	flags_arr[0] = a;

	update_n_flag(1, flags_arr);
	update_z_flag(1, flags_arr);
	
}

inline void lda_l_x() {
	address = rom[++pc];
	byte_low = rom[++pc];
	byte_high = rom[++pc];

	address = ((byte_high << 16 | byte_low << 8 | address) + x) & 0xFFFFFF;

	a = total_memory[address];
	flags_arr[0] = a;
	update_n_flag(1, flags_arr);
	update_z_flag(1, flags_arr);
}

inline void lda_l() {
	address = rom[++pc];
	byte_low = rom[++pc];
	byte_high = rom[++pc];

	address = (byte_high << 16 | byte_low << 8 | address) & 0xFFFFFF;

	a = total_memory[address];
	flags_arr[0] = a;
	update_n_flag(1, flags_arr);
	update_z_flag(1, flags_arr);
}


inline void lda_addr_y() {
	address = rom[++pc];
	a = total_memory[address + y];
	
	flags_arr[0] = a;

	update_n_flag(1, flags_arr);
	update_z_flag(1, flags_arr);
}
inline void lda_addr_x() {
	address = rom[++pc];
	a = total_memory[address + x];
	flags_arr[0] = a;

	update_n_flag(1, flags_arr);
	update_z_flag(1, flags_arr);
}

inline void ldx_dp() {
	address = rom[++pc] + dp;
	x = total_memory[address];

	flags_arr[0] = x;

	update_n_flag(1, flags_arr);
	update_z_flag(1, flags_arr);
}

inline void ldx_const() {
	address = rom[++pc];
	byte_low = rom[++pc];
	byte_high = rom[++pc];

	if (!x_flag)
		address |= (byte_high << 16 | byte_low << 8);
	else
		address = ((byte_low << 8 | address) & 0xFF);

	x = address;

	flags_arr[0] = x;
	update_n_flag(1, flags_arr);
	update_z_flag(1, flags_arr);
}

inline void ldx_addr() {
	byte_low = rom[++pc];
	byte_high = rom[++pc];

	address = (byte_high << 8 | byte_low);

	x = total_memory[address];

	flags_arr[0] = x;

	update_n_flag(1, flags_arr);
	update_z_flag(1, flags_arr);
}

inline void ldx_addr_y() {
	byte_low = rom[++pc];
	byte_high = rom[++pc];

	address = (byte_high << 8 | byte_low) + y;
	x = total_memory[address];

	flags_arr[0] = x;

	update_n_flag(1, flags_arr);
	update_z_flag(1, flags_arr);
}


inline void ldx_dp_y() {
	address = rom[++pc];
	x = total_memory[address + dp + y];

	flags_arr[0] = x;

	update_n_flag(1, flags_arr);
	update_z_flag(1, flags_arr);
}

inline void ldy_addr() {
	address = rom[++pc];
	y = total_memory[address];

	flags_arr[0] = y;

	update_n_flag(1, flags_arr);
	update_z_flag(1, flags_arr);
}

inline void ldy_addr_x() {
	address = rom[++pc];
	y = total_memory[address + x];

	flags_arr[0] = y;

	update_n_flag(1, flags_arr);
	update_z_flag(1, flags_arr);
}

inline void ldy_const() {
	byte_low = rom[++pc];
	byte_high = rom[++pc];

	if (x_flag) {
		address = rom[++pc];
	} else {
		address = (byte_high << 8 | byte_low);
	}
	y = address;

	flags_arr[0] = y;

	update_n_flag(1, flags_arr);
	update_z_flag(1, flags_arr);
}

inline void lsr_dp() {
	address = rom[++pc] + dp;
	c_flag |= (0x01 & total_memory[address]);

	total_memory[address] >>= 1;
	flags_arr[0] = total_memory[address];

	update_n_flag(1, flags_arr);
	update_z_flag(1, flags_arr);
}

inline void lsr_a() {
	c_flag |= (0x01 & a);
	a >>= 1;	
	flags_arr[0] = a;

	update_n_flag(1, flags_arr);
	update_z_flag(1, flags_arr);
}

inline void lsr_addr() {
	byte_low = rom[++pc];
	byte_high = rom[++pc];
	address = (byte_high << 8 | byte_low);

	c_flag |= (0x01 & total_memory[address]);
	total_memory[address] >>= 1;
	
	flags_arr[0] = total_memory[address];

	update_n_flag(1, flags_arr);
	update_n_flag(1, flags_arr);
}

inline void lsr_dp_x() {
	address = rom[++pc] + dp + x;

	c_flag |= (0x01 & total_memory[address]);
	total_memory[address] >>= 1;
	
	flags_arr[0] = total_memory[address];

	update_n_flag(1, flags_arr);
	update_n_flag(1, flags_arr);
}

inline void lsr_addr_x() {
	byte_low = rom[++pc];
	byte_high = rom[++pc];

	c_flag |= (0x01 & total_memory[address]);
	address = (byte_high << 8 | byte_low) + x;

	(total_memory[address] >>= 1);
	flags_arr[0] = total_memory[address];

	update_n_flag(1, flags_arr);
	update_n_flag(1, flags_arr);
}

inline void ldy_dp() {
	address = rom[++pc];
	y = total_memory[address + dp];

	flags_arr[0] = y;

	update_n_flag(1, flags_arr);
	update_z_flag(1, flags_arr);
}

inline void ldy_dp_x() {
	address = rom[++pc];
	y = total_memory[address + dp + x];

	flags_arr[0] = y;

	update_n_flag(1, flags_arr);
	update_z_flag(1, flags_arr);
}

inline void ora_dp() {
	address = rom[++pc];
	a = (total_memory[address + dp] | a);	
	flags_arr[0] = a;
	
	update_n_flag(1, flags_arr);
	update_z_flag(1, flags_arr);
}

inline void ora_dp_indr() {
	address = rom[++pc] + dp;

	byte_low = (total_memory[address]);
	byte_high = (total_memory[address + 1]);

	address = (byte_high << 8 | byte_low);
	operation = (a ^ total_memory[address]);

	flags_arr[0] = operation;

	update_n_flag(1, flags_arr);
	update_z_flag(1, flags_arr);
}

inline void ora_dp_indr_y() {
	address = rom[++pc] + dp;

	byte_low = (total_memory[address]);
	byte_high = (total_memory[address + 1]);

	address = (byte_high << 8 | byte_low) + y;
	operation = (a ^ total_memory[address]);

	flags_arr[0] = operation;

	update_n_flag(1, flags_arr);
	update_z_flag(1, flags_arr);
}

inline void ora_const() {
	// This function basically checks if it's 8 bit mode
	// if yes do ORA in 8 bit mode, else adds 1 cycle
	if (!m_flag)  {
		byte_low = rom[++pc];
		byte_high = rom[++pc];

		a = ((byte_high << 16 | byte_low << 8) | a);
	} else {
		a = (rom[++pc] | a);
	}

	flags_arr[0] = a;
	
	update_n_flag(1, flags_arr);
	update_z_flag(1, flags_arr);
}


inline void ora_dp_x() {
	address = rom[++pc];
	a = (total_memory[address + dp + x] | a);	
	flags_arr[0] = a;
	
	update_n_flag(1, flags_arr);
	update_z_flag(1, flags_arr);
}

inline void ora_dp_x_indr() {
	address = (dp + x);

	byte_low = total_memory[address];
	byte_high = total_memory[address + 1];
	
	address = (byte_high << 8 | byte_low);

	operation = (a | total_memory[address]);
	flags_arr[0] = operation;
	update_n_flag(1, flags_arr);
	update_z_flag(1, flags_arr);
}

inline void ora_sr_s() {
	address = rom[++pc] + ((i8) stack[s]);
	a = (address | a);
	flags_arr[0] = a;
	
	update_n_flag(1, flags_arr);
	update_z_flag(1, flags_arr);
}

inline void ora_addr() {
	byte_low = rom[++pc];
	byte_high = rom[++pc];

	address = (byte_high << 8 | byte_low);

	a = (total_memory[address] | a);
	flags_arr[0] = a;
	
	update_n_flag(1, flags_arr);
	update_z_flag(1, flags_arr);
}

inline void ora_addr_y() {
	offset = rom[++pc];
	byte_low = offset + rom[++pc];
	byte_high = offset + rom[++pc];

	address = (byte_high << 8 | byte_low);

	a = (total_memory[address + y] | a);
	flags_arr[0] = a;
	
	update_n_flag(1, flags_arr);
	update_z_flag(1, flags_arr);
}

inline void ora_addr_x() {
	byte_low = rom[++pc];
	byte_high = rom[++pc];

	address = (byte_high << 8 | byte_low);

	a = (total_memory[address + x] | a);
	flags_arr[0] = a;
	
	update_n_flag(1, flags_arr);
	update_z_flag(1, flags_arr);
}

inline void pea() {
	byte_low = rom[++pc];
	byte_high = rom[++pc];

	address = (byte_high << 8 | byte_low);

	stack[s - 2] = address;
}

inline void pei_dp() {
	printf("Not implemented PEI \n");
}

inline void per() {
	printf("Not implemented PER \n");
}

inline void pld() {
	printf("Not implemented PLD \n");
	return;	
}


inline void pha() {
/*	if (m_flag) {*/
		stack[s--] = a;
/*	} else {
		byte_low = (a & 0xFF);
		byte_high = a >> 8;
		stack[s] = byte_low;
		stack[--s] = byte_high;
	}*/
	flags_arr[0] = stack[++s];

	update_n_flag(1, flags_arr);
	update_z_flag(1, flags_arr);
}

inline void phb() {
/*	if (m_flag) {*/
		stack[s--] = db;
/*	} else {
		byte_low = (db & 0xFF);
		byte_high = db >> 8;
		stack[s] = byte_low;
		stack[--s] = byte_high;
	}*/
	flags_arr[0] = stack[++s];

	update_n_flag(1, flags_arr);
	update_z_flag(1, flags_arr);
}


inline void phd() {
/*	if (m_flag) {*/
		stack[s--] = dp;
/*	} else {
		byte_low = (dp & 0xFF);
		byte_high = dp >> 8;
		stack[s] = byte_low;
		stack[--s] = byte_high;
	}*/
	flags_arr[0] = stack[++s];

	update_n_flag(1, flags_arr);
	update_z_flag(1, flags_arr);
}

inline void phk() {
	/*if (m_flag) {*/
		stack[s--] = pb;
	/*} else {
		byte_low = (pb & 0xFF);
		byte_high = pb >> 8;
		stack[s] = byte_low;
		stack[--s] = byte_high;
	}*/
	flags_arr[0] = stack[++s];

	update_n_flag(1, flags_arr);
	update_z_flag(1, flags_arr);
}

inline void phx() {
	/*if (m_flag) { Though there was going to have a use for 16-bit, but no, i was wrong*/
		if (verbose)
			printf("8-bit mode \n");
		stack[s--] = x;
	/*}  
	else {
		if (verbose)
			printf("16-bit mode \n");
		byte_low = (x & 0xFF);
		byte_high = x >> 8;
		stack[s] = byte_low;
		stack[--s] = byte_high;
	}*/
	flags_arr[0] = stack[++s];

	update_n_flag(1, flags_arr);
	update_z_flag(1, flags_arr);
}

inline void phy() {
	/*if (m_flag) {*/
		stack[s--] = y;
	/*} else {
		byte_low = (y & 0xFF);
		byte_high = y >> 8;
		stack[s] = byte_low;
		stack[--s] = byte_high;
	}*/
	flags_arr[0] = stack[++s];

	update_n_flag(1, flags_arr);
	update_z_flag(1, flags_arr);
}

inline void pla() {
	a = stack[s++];
	
	flags_arr[0] = stack[s];
	update_n_flag(1, flags_arr);
	update_z_flag(1, flags_arr);
}

inline void plb() {
	db = stack[s++];

	flags_arr[0] = stack[--s];
	update_n_flag(1, flags_arr);
	update_z_flag(1, flags_arr);

}

inline void php() {
	// I know this is wrong setting a fixed-value isn' right
	p |= (n_flag & 0x80);
	p |= (v_flag & 0x40);
	p |= (m_flag & 0x20);
	p |= (x_flag & 0x10);
	p |= (d_flag & 0x08);
	p |= (i_flag & 0x04);
	p |= (z_flag & 0x02);
	p |= (c_flag & 0x01);

	//stack[s] = ((stack[s] << 7) & n_flag);
	stack[--s] = p;

	/* This works (probally) but inefficient
	stack[s] = (stack[s] & n_flag);
	stack[s] = (stack[s] & v_flag);
	stack[s] = (stack[s] & m_flag);
	stack[s] = (stack[s] & x_flag);
	stack[s] = (stack[s] & d_flag);
	stack[s] = (stack[s] & i_flag);
	stack[s] = (stack[s] & c_flag);
	*/
}

inline void plp() {
	p = stack[s++]; 
}

inline void plx() {
	x = stack[++s];
	flags_arr[0] = stack[s];
	
	s --;
	update_n_flag(1, flags_arr);
	update_z_flag(1, flags_arr);
}

inline void ply() {
	y = stack[++s];

	flags_arr[0] = stack[s];
	s --;
	update_n_flag(1, flags_arr);
	update_z_flag(1, flags_arr);
}

inline void rep() {
	address = rom[++pc];

	if (address & 0x80)
		n_flag = 0;
	if (address & 0x40)
		v_flag = 0;
	if (address & 0x20)
		m_flag = 0;
	if (address & 0x10)
		x_flag = 0;
	if (address & 0x08)
		d_flag = 0;
	if (address & 0x04)
		i_flag = 0;
	if (address & 0x02)
		z_flag = 0;
	if (address & 0x01)
		c_flag = 0;
}

inline void rol_a() {
	address = rom[++pc];
	//Ensure temporary equals to zero
	temp_bit = 0;
	temp_bit |= (c_flag);
	a <<= 1;

	if (temp_bit & 0x01) {
		a = (u16) a | temp_bit;
	}

	update_n_flag(1, flags_arr);
	update_z_flag(1, flags_arr);
}

inline void ror_a() {
	address = rom[++pc];
	//Ensure temporary equals to zero
	temp_bit = 0;
	temp_bit |= c_flag;
	a >>= 1;

	if (temp_bit & 0x01) {
		a = (u16) a | temp_bit;
	}

	update_n_flag(1, flags_arr);
	update_z_flag(1, flags_arr);
}

inline void rol_addr() {
	address = rom[++pc];
	byte_low = rom[++pc];
	byte_high = rom[++pc];

	address = (byte_high << 16 | byte_low << 8 | address);
	//Ensure temporary equals to zero
	temp_bit = 0;
	temp_bit |= (c_flag);
	total_memory[address] <<= 1;

	if (temp_bit & 0x01) {
		total_memory[address] = (u16) total_memory[address] | temp_bit;
	}

	update_n_flag(1, flags_arr);
	update_z_flag(1, flags_arr);
}

inline void ror_addr() {
	address = rom[++pc];
	byte_low = rom[++pc];
	byte_high = rom[++pc];

	address = (byte_high << 16 | byte_low << 8 | address);
	//Ensure temporary equals to zero
	temp_bit = 0;
	temp_bit |= (c_flag);
	total_memory[address] >>= 1;

	if (temp_bit & 0x01) {
		total_memory[address] =  (u16) total_memory[address] | temp_bit;
	}

	update_n_flag(1, flags_arr);
	update_z_flag(1, flags_arr);
}

inline void rol_addr_x() {
	address = rom[++pc];
	byte_low = rom[++pc];
	byte_high = rom[++pc];

	address = (byte_high << 16 | byte_low << 8 | address) + x;

	//Ensure temporary equals to zero
	temp_bit = 0;
	temp_bit |= (c_flag);
	total_memory[address] <<= 1;

	if (temp_bit & 0x01) {
		total_memory[address] |= (u16) total_memory[address] | temp_bit;
	}

	update_n_flag(1, flags_arr);
	update_z_flag(1, flags_arr);
}

inline void ror_addr_x() {
	address = rom[++pc];
	byte_low = rom[++pc];
	byte_high = rom[++pc];

	address = (byte_high << 16 | byte_low << 8 | address) + x;

	//Ensure temporary equals to zero
	temp_bit = 0;
	temp_bit |= (c_flag);
	total_memory[address] >>= 1;

	if (temp_bit & 0x01) {
		total_memory[address] = (u16) total_memory[address] | temp_bit;
	}

	update_n_flag(1, flags_arr);
	update_z_flag(1, flags_arr);
}

inline void rol_dp() {
	address = rom[++pc] + dp;
	//Ensure temporary equals to zero
	temp_bit = 0;
	temp_bit |= (c_flag);
	total_memory[address] <<= 1;

	if (temp_bit & 0x01) {
		total_memory[address] = (u16) total_memory[address] | temp_bit;
	}

	update_n_flag(1, flags_arr);
	update_z_flag(1, flags_arr);
}

inline void ror_dp() {
	address = rom[++pc] + dp;
	//Ensure temporary equals to zero
	temp_bit = 0;
	temp_bit |= (c_flag);
	total_memory[address] >>= 1;

	if (temp_bit & 0x01) {
		total_memory[address] = (u16) total_memory[address] | temp_bit;
	}

	update_n_flag(1, flags_arr);
	update_z_flag(1, flags_arr);
}

inline void rol_dp_x() {
	address = rom[++pc] + dp + x;
	//Ensure temporary equals to zero
	temp_bit = 0;
	temp_bit |= (c_flag);
	total_memory[address] <<= 1;

	if (temp_bit & 0x01) {
		total_memory[address] = (u16) total_memory[address] | temp_bit;
	}

	update_n_flag(1, flags_arr);
	update_z_flag(1, flags_arr);
}

inline void ror_dp_x() {
	address = rom[++pc] + dp + x;
	//Ensure temporary equals to zero
	temp_bit = 0;
	temp_bit |= (c_flag);
	total_memory[address] >>= 1;

	if (temp_bit & 0x01) {
		total_memory[address] = (u16) total_memory[address] | temp_bit;
	}

	update_n_flag(1, flags_arr);
	update_z_flag(1, flags_arr);
}

inline void rts() {
	address = stack[pc];
	byte_low = stack[pc + 1];
	byte_high = stack[pc + 2];

	address = ((byte_high << 16 | byte_low << 8 | address) + 1);

	pc = saved_pc + 1;
}

inline void tax() {
	x = a;

	flags_arr[0] = x;

	update_n_flag(1, flags_arr);
	update_z_flag(1, flags_arr);
}

inline void tay() {
	y = a;

	flags_arr[0] = y;

	update_n_flag(1, flags_arr);
	update_z_flag(1, flags_arr);
}

inline void tcd() {
	dp = a;
	
	flags_arr[0] = dp;

	update_n_flag(1, flags_arr);
	update_z_flag(1, flags_arr);
//update_flags(dp);
}

inline void tcs() {
	s = a;

	flags_arr[0] = s;

	update_n_flag(1, flags_arr);
	update_z_flag(1, flags_arr);
}

inline void tdc() {
	a = dp;

	flags_arr[0] = a;
	update_n_flag(1, flags_arr);
	update_z_flag(1, flags_arr);
}

inline void trb_dp() {
	address = rom[++pc] + dp;

	operation = (total_memory[address] & a);

	total_memory[address] = (total_memory[address] & operation);
	flags_arr[0] = operation;
	update_z_flag(1, flags_arr);
}

inline void trb_addr() {
	byte_low = rom[++pc];
	byte_high = rom[++pc];

	address = (byte_high << 8 | byte_low) + dp;

	operation = (total_memory[address] & a);

	total_memory[address] = (total_memory[address] & operation);
	flags_arr[0] = operation;
	update_z_flag(1, flags_arr);
}

inline void tsb_dp() {
	address = rom[++pc] + dp;
	operation = (total_memory[address] & a);

	flags_arr[0] = operation;
	total_memory[address] = operation;
	update_z_flag(1, flags_arr);
}

inline void tsb_addr() {
	byte_low = rom[++pc];
	byte_high = rom[++pc];

	address = (byte_high << 8 | byte_low);
	operation = (total_memory[address] & a);

	flags_arr[0] = operation;
	update_z_flag(1, flags_arr);

	total_memory[address] = (a | total_memory[address]);
}

inline void tsc() {
	a = s;

	flags_arr[0] = a;
	update_n_flag(1, flags_arr);
	update_z_flag(1, flags_arr);
}

	inline void tsx() {
	x = s;
	flags_arr[0] = x;
	update_n_flag(1, flags_arr);
	update_z_flag(1, flags_arr);
}

inline void txa() {
	a = x;
	flags_arr[0] = a;
	update_n_flag(1, flags_arr);
	update_z_flag(1, flags_arr);
}

inline void txs() {
	x = x;
}

inline void txy() {
	y = x;

	flags_arr[0] = y;
	update_n_flag(1, flags_arr);
	update_z_flag(1, flags_arr);
}

inline void tya() {
	a = y;
	flags_arr[0] = a;
	update_n_flag(1, flags_arr);
	update_z_flag(1, flags_arr);
}

inline void tyx() {
	x = y;
	flags_arr[0] = x;
	update_n_flag(1, flags_arr);
	update_z_flag(1, flags_arr);
}

inline void sbc_dp_x_indr() {
	address = rom[++pc] + dp + x;

	byte_low = total_memory[address];
	byte_high = total_memory[address + 1];

	address = (byte_high << 8 | byte_low);

	a -= (total_memory[address] - (!c_flag ? 1 : 0));
	flags_arr[0] = a;
	update_n_flag(1, flags_arr);
	update_z_flag(1, flags_arr);
}

inline void sbc_sr_s_indr_y() {
	byte_low = ((stack[s]) & 0xFF);
	byte_high = ((stack[s + 1]) & 0xFF);

	address = (byte_high << 8 | byte_low) + y;
	a -= (total_memory[address] - (!c_flag ? 1 : 0));

	flags_arr[0] = a;
	update_n_flag(1, flags_arr);
	update_z_flag(1, flags_arr);
}


inline void sbc_sr_s() {
	a -= (stack[s] - (!c_flag ? 1 : 0));

	flags_arr[0] = a;
	update_n_flag(1, flags_arr);
	update_z_flag(1, flags_arr);
}

inline void sbc_dp() {
	address = rom[++pc] + dp;

	a -= (total_memory[address] - (!c_flag ? 1 : 0));
	
	flags_arr[0] = a;
	update_n_flag(1, flags_arr);
	update_z_flag(1, flags_arr);
}

inline void sbc_dp_indr_l() {
	printf("SBC DP Indr Long \n");
	offset = rom[++pc] + dp;

	address = total_memory[offset];
	byte_low = total_memory[offset + 1];
	byte_high = total_memory[offset + 2];

	address = ((byte_high << 16) | (byte_low << 8) | address);

	a -= (total_memory[address] - (!c_flag ? 1 : 0));
	flags_arr[0] = a;
	update_n_flag(1, flags_arr);
	update_z_flag(1, flags_arr);
}

inline void sbc_dp_indr_l_y() {
	offset = rom[++pc] + dp;
	address = (total_memory[offset]);
	byte_low = (total_memory[offset + 1]);
	byte_high = (total_memory[offset + 2]);

	address = (byte_high << 16 | byte_low << 8 | address) + y;
	
	if (verbose) {
		if (total_memory[address] & 0xffffff) {
			printf("Value is 24-bits \n");
		}
	}

	a -= (total_memory[address] - (!c_flag));

	flags_arr[0] = a;
	update_n_flag(1, flags_arr);
	update_z_flag(1, flags_arr);
	printf("Finished \n");
}

inline void sbc_const() {
	byte_low = rom[++pc];
	byte_high = rom[++pc];

	if (m_flag) {
		address = (byte_high << 8 | byte_low);
	} else {
		address = byte_high;
	}

	a -= (address - !(c_flag));

	flags_arr[0] = operation;

	update_n_flag(1, flags_arr);
	update_z_flag(1, flags_arr);
}

inline void sbc_dp_x() {
	address = rom[++pc] + dp + x;

	a -= (total_memory[address] - (!c_flag ? 1 : 0));
	
	flags_arr[0] = a;
	update_n_flag(1, flags_arr);
	update_z_flag(1, flags_arr);
}

inline void sbc_addr() {
	byte_low = rom[++pc];
	byte_high = rom[++pc];

	address = (byte_high << 8 | byte_low);

	a -= (total_memory[address] - (!c_flag ? 1 : 0));
	
	flags_arr[0] = a;
	update_n_flag(1, flags_arr);
	update_z_flag(1, flags_arr);
}

inline void sbc_addr_y() {
	byte_low = rom[++pc];
	byte_high = rom[++pc];

	address = (byte_high << 8 | byte_low) + y;

	a -= (total_memory[address] - (!c_flag ? 1 : 0));
	
	flags_arr[0] = a;
	update_n_flag(1, flags_arr);
	update_z_flag(1, flags_arr);
}

inline void sbc_addr_x() {
	byte_low = rom[++pc];
	byte_high = rom[++pc];

	address = (byte_high << 8 | byte_low) + x;

	a -= (total_memory[address] - (!c_flag ? 1 : 0));
	
	flags_arr[0] = a;
	update_n_flag(1, flags_arr);
	update_z_flag(1, flags_arr);
}

inline void sbc_dp_indr() {
	address = rom[++pc] + dp;

	byte_low = total_memory[address];
	byte_high = total_memory[address + 1];

	address = ((byte_high << 8 | byte_low)); //Ensuring it doesn't passes 16-bit
	a -= (total_memory[address] - !(c_flag ? 1 : 0));

	flags_arr[0] = a;
	update_n_flag(1, flags_arr);
	update_z_flag(1, flags_arr);
}

inline void sbc_dp_indr_y() {
	address = rom[++pc] + dp;

	byte_low = total_memory[address];
	byte_high = total_memory[address + 1];

	address = (byte_high << 8 | byte_low) + y;

	a -= (total_memory[address] - (!c_flag ? 1 : 0));
	flags_arr[0] = a;
	update_n_flag(1, flags_arr);
	update_z_flag(1, flags_arr);
}

inline void sbc_l() {
	address = rom[++pc];
	byte_low = rom[++pc];
	byte_high = rom[++pc];

	address = ((byte_high << 16 | byte_low << 8 | address) & 0xFFFFFF);

	a &= (0xFFFF - address);
	flags_arr[0] = a;

	update_n_flag(1, flags_arr);
	update_z_flag(1, flags_arr);
}

inline void sbc_l_x() {
	address = rom[++pc];
	byte_low = rom[++pc];
	byte_high = rom[++pc];

	address = (((u32) byte_high) << 16 | ((u32) byte_low) << 8 | address) + x;

	a -= (total_memory[address] - (!c_flag ? 1 : 0));

	flags_arr[0] = a;
	update_n_flag(1, flags_arr);
	update_z_flag(1, flags_arr);
}

inline void sec() {
// this flag checks the address overflows
// the data type like u16 being in u8 in add
// operations 
	c_flag = 1;
}

inline void sed() {
// this is not as simple as simply checking
// this flag checks the mode of how add and sub operations are made
	d_flag = 1;
}

inline void sei() {
	i_flag = 1;
}

inline void sep() {
	//Kinda hard to explain that one
	//But basically each bit is a flag

	address = rom[++pc];
	n_flag = ((address & 0x80) ? 1 : 0);
	v_flag = ((address & 0x40) ? 1 : 0);
	m_flag = ((address & 0x20) ? 1 : 0);
	x_flag = ((address & 0x10) ? 1 : 0);
	d_flag = ((address & 0x08) ? 1 : 0);
	i_flag = ((address & 0x04) ? 1 : 0);
	z_flag = ((address & 0x02) ? 1 : 0);
	c_flag = ((address & 0x01) ? 1 : 0);
}

inline void sta_sr_s() {
	address = (rom[++pc] + ((i8) s));
	total_memory[address] = (u16) a;
}

inline void sta_dp() {
	byte_low = rom[++pc];
	byte_high = rom[++pc];

	if (!m_flag)
		address = (byte_high << 8 | byte_low);
	else
		address = (byte_low);

	address = rom[++pc] + dp;
	total_memory[address] = (u16) a;
}

//check these 2
inline void sta_dp_indr_l() {
	offset = rom[++pc];

	address = offset + dp;
	byte_low = (total_memory[address + 1]);
	byte_high = (total_memory[address + 2]);

	total_memory[address] |= (byte_high << 16 | byte_low << 8);
	total_memory[address] = (u16) a;
}

inline void sta_dp_indr_l_y() {
	offset = rom[++pc];

	address = offset + dp;
	byte_low = (total_memory[address + 1]);
	byte_high = (total_memory[address + 2]);

	address = (byte_high << 16 | byte_low << 8 | total_memory[address]) + y;
	total_memory[address] = a;
}

inline void sta_dp_indr() {
	address = rom[++pc] + dp;
	byte_low = (total_memory[address]);
	byte_high = (total_memory[address + 1]);

	address = (byte_high << 8 | byte_low);
	total_memory[address] = a;
}

inline void sta_addr() {
	byte_low = rom[++pc];
	byte_high = rom[++pc];

	address = (byte_high << 8 | byte_low);

	total_memory[address] = a;
}

inline void sta_addr_x() {
	byte_low = rom[++pc];
	byte_high = rom[++pc];

	address = (byte_high << 8 | byte_low) + x;

	total_memory[address] = a;
}

inline void sta_addr_y() {
	byte_low = rom[++pc];
	byte_high = rom[++pc];

	address = (byte_high << 8 | byte_low) + y;

	total_memory[address] = a;
}

inline void sta_l() {
	address = rom[++pc];
	byte_low = rom[++pc];
	byte_high = rom[++pc];

	address = (byte_high << 16 | byte_low << 8 | address);
	total_memory[address] = a;
}

inline void sta_l_x() {
	address = rom[++pc];
	byte_low = rom[++pc];
	byte_high = rom[++pc];

	address = ((u32) byte_high << 16 | (u32) byte_low << 8 | address) + x;
	total_memory[address] = a;
}

inline void sta_dp_x() {
	address = rom[++pc] + dp + y;
	total_memory[address] = a;
}

inline void sta_sr_s_y() {
	address = (rom[++pc] + s);
	byte_low = total_memory[address];
	byte_high = total_memory[address + 1];
	
	address = (byte_low | byte_high << 8) + y;
	total_memory[address] = a;
	return;
}

inline void sta_dp_x_indr() {
	offset = (rom[++pc] + dp + x);
	byte_low = total_memory[offset];
	byte_high = total_memory[offset + 1];
	address = (byte_low | byte_high << 8);
	total_memory[address] = a;
	return;
}

inline void sta_dp_indr_y() {
	offset = (rom[++pc] + dp);
	byte_low = total_memory[offset];
	byte_high = total_memory[offset + 1];
	address = (byte_low | byte_high << 8) + y;
	total_memory[address] = a;
	return;
}

inline void stp() {
	printf("CPU Halted, waiting for reset, did something goes wrong? \n");
	cpu_waiting = 0;
	return;
}

inline void stx_dp() {
	address = rom[++pc] + dp;
	total_memory[address] = x;
};

inline void stx_addr() {
	byte_low = rom[++pc];
	byte_high = rom[++pc];

	address = (byte_high << 8 | byte_low);
	total_memory[address] = x;
}

inline void stx_dp_y() {
	address = rom[++pc] + dp + y;
	total_memory[address] = x;
}

inline void sty_dp() {
	address = rom[++pc] +dp;
	total_memory[address] = y;
}

inline void sty_addr() {
	byte_low = rom[++pc];
	byte_high = rom[++pc];

	address = (byte_high << 8 | byte_low);

	address = rom[++pc];
	total_memory[address] = y;
}

inline void sty_dp_x() {
	address = rom[++pc] + dp + x;

	total_memory[address] = y;
};

inline void stz_dp() {
	address = rom[++pc] + dp;

	total_memory[address] = 0;
};

inline void stz_dp_x() {
	address = rom[++pc] + dp + x;

	total_memory[address] = 0;
};

inline void stz_addr() {
	byte_low = rom[++pc];
	byte_high = rom[++pc];

	address = ((byte_high << 8 | byte_low) & 0xFFFF);

	total_memory[address] = 0;
};

inline void stz_addr_x() {
	byte_low = rom[++pc];
	byte_high = rom[++pc];

	address = (byte_high << 8 | byte_low) + x;

	total_memory[address] = 0;
};

inline void wai() {
	cpu_waiting = 0;
}

inline void wdm() {
	/* Thank you for losting my whole searching
	 * you what your stupid opcode does! Nothing*/
	printf("Doing absolutely NOTHING :) WDM \n");

	/* Here is your reserved space, little nigga*/
}

inline void xba() {
	// Example
	// a = 1580; 
	//
	// byte_low  = a >> 8 = 0015
	// byte_high = a << 8 = 8000
	//
	// byte_high | byte_low = 8015;
	//
	byte_low = (a >> 8);
	byte_high = (a << 8);
	a = ((byte_high << 8 | byte_low) & 0xFFFF);

	flags_arr[0] = operation;
	update_n_flag(1, flags_arr);
	update_z_flag(1, flags_arr);
}

inline void xce() {
	e_flag = !(c_flag);
	c_flag = !(e_flag);
}
/*
inline void wai() {
cpu_waiting = 1;
}
/*
inline void xce() {
//im too tired to implement this now, sorry
//and its wrong btw
c_flag = ((e_flag == 0) ? 1 : 0);
e_flag = ((c_flag == 1) ? 0 : 1);
}*/

inline void update_z_flag(int count, u16 operations[]) {
	switch (count) {
		case 1:
			z_flag = ((operations[0] == 0) ? 1 : 0);
		break;

		case 2:
			z_flag = ((operations[0] & operations[1] == 0) ? 1 : 0);
		break;

		default:
			printf("Error, too much arguments \n");
		break;
	}
}

inline void update_c_flag(u16 operations[]) {
	// I have no idea how to code this 
	// But let's go :)
	// This flag is used as an 'pile' cause of
	// SNES compatiblity mode(shit)
	// So if the address exceeds u8 size(2 bytes)
	// this values gets to C_FLAG :)
	c_flag = ((operations[0] & 0x00) ? 1 : 0);
};

inline void update_m_flag(u16 operation) {
//This flag can only be used in native-mode(16 bit)
	if (!e_flag) {
		m_flag = ((operation & 0x20) ? 1 : 0);
	} else {
		printf("Error, we're on 16 bit mode");
	}
}
/*
inline void update_c_flag(int count, u16 operations[]) {
	switch (count) {
		case 1:
		break;

		case 2:

		break;
		default:
			printf("Not implemented \n");
		break;
	};
}
*/
inline void update_n_flag(int count, u16 operations[]) {
	if (x_flag) {
		switch (count) {
			case 1:
				if (verbose) {
					printf("1: Updating N \n");
				}
				n_flag = ((operations[0] & 0x80) ? 1 : 0);
			break;
			
			case 2:
				u16 new_operat = (operations[0] & operations[1]);
				if (verbose) {
					printf("2: Updating N \n");
				}
				n_flag = ((new_operat & 0x80) ? 1 : 0);
			break;
			default:
				printf("Error, too much arguments \n");
			break;
		}
	} else {
		switch (count) {
			case 1:
				n_flag = ((operations[0] & 0x8000) ? 1 : 0);
				if (verbose) {
					printf("1: Updating Z \n");
				}
			break;

			case 2:
				u16 new_operat = (operations[0] & operations[1]);
				n_flag = ((new_operat & 0x8000) ? 1 : 0);
				if (verbose) {
					printf("2: Updating Z \n");
				}
			break;
			default:		
				printf("Error, too much arguments \n");
			break;
		}
	}
}



/*extern void update_z_flag(u16 operation) {
z_flag = ((operation == 0) ? 1 : 0); //0x02 is the value of z
}*/

//This function is not used anymore
inline void update_flags(u16 operation) {	
	m_flag = ((operation & 0x20) ? 1 : 0); //not implemented but have the check
	//(e_flag == 0 ? 1 : 0);
}

//This is a very important function, that will make 16 bit addresses, basically merging
//them via bitwise(love u bitwise)
inline int mergeBytes() {
	byte_low = rom[++pc];
	byte_high = rom[++pc];

	//Basically we get the highest byte, move 8 bits to the left
	//and use | operator to merge the bits correctly
	address = (byte_high << 8 | byte_low);

	return address;
}
