#include <stdio.h>
#include "defs.h"

// prefix
// _dp_x
// _sr_s
// _dp
// _dp_direct
// _const
// _addr
// _long
// _dp_y 
// _dp_indr 
// _sr_s_y
// _dp_x_ind 
// _dp_y_ind
// _addr_y
// _addr_x
// _long_x


// gpt gave a good reason to think lmao
enum emulator_instructions {
	_add = 0xFFFF, // add exatcle ONE value to memory
	_mov = 0xFFFB, // move/copy an address to another
	_prn = 0xFFFC, // print one address to another
	_eoq = 0xFFFD, // end or quit
	_ato = 0xFFFE, // suppose to print the corresponding char of a hexadecimal
};

enum cpu_instructions {
	//ADD INS
	_adc_dp_x	= 0x61,
	_adc_sr_s 	= 0x63,
	_adc_dp		= 0x65,
	_adc_dp_indr_l	= 0x67,
	_adc_const	= 0x69,
	_adc_addr 	= 0x6D,
	_adc_long 	= 0x6F,
	
	_adc_dp_y 	= 0x71,
	_adc_dp_indr 	= 0x72,
	_adc_sr_s_y 	= 0x73,
	_adc_dp_x_ind	= 0x75,
	_adc_dp_y_ind 	= 0x77,
	_adc_addr_y 	= 0x79,
	_adc_addr_x 	= 0x7D,
	_adc_long_x 	= 0x7F,
			
	// AND INS
	_and_dp_x	= 0x21,
	_and_sr_s 	= 0x23,
	_and_dp		= 0x25,
	_and_dp_indr_l	= 0x27,
	_and_const	= 0x29,
	_and_addr 	= 0x2D,
	_and_long 	= 0x2F,

	_and_dp_y 	= 0x31,
	_and_dp_indr 	= 0x32,
	_and_sr_s_y 	= 0x33,
	_and_dp_x_ind	= 0x35,
	_and_dp_y_ind 	= 0x37,
	_and_addr_y 	= 0x39,
	_and_addr_x 	= 0x3D,
	_and_long_x 	= 0x3F,

	// ASL INS
	_asl_dp		= 0x06,
	_asl_a		= 0x0A,
	_asl_addr	= 0x0E,
	_asl_dp_x	= 0x16,
	_asl_addr_x	= 0x1E,

	// BCC BLT BSS BGE INS
	_bcc		= 0x90,
	_blt		= 0x90, //alias
	_bss		= 0xB0,
	_bge		= 0xB0, //alias
	
	// BEQ INS
	_beq		= 0xF0,
	
	// BIT INS	
	_bit_dp		= 0x24,
	_bit_addr	= 0x2C,
	_bit_dp_x	= 0x34,
	_bit_addr_x	= 0x3C,
	_bit_const	= 0x89,

	// BMI BNE BMP BRA BRK INS
	_bmi		= 0x30,
	_bne		= 0xD0,
	_bmp		= 0x10,
	_bra		= 0x80,
	_brk		= 0x00, //break

	// BVC BVS CLC CLD CLI CLV INS
	_bvc		= 0x50,
	_bvs		= 0x70,
	_clc		= 0x18,
	_cld		= 0xD8,
	_cli		= 0x58,
	_clv		= 0xB8,

	// CMP INS
	_cmp_dp_x	= 0xC1,
	_cmp_sr_s 	= 0xC3,
	_cmp_dp		= 0xC5,
	_cmp_dp_direct	= 0xC7,
	_cmp_const	= 0xC9,
	_cmp_addr 	= 0xCD,
	_cmp_long 	= 0xCF,

	_cmp_dp_y	= 0xD1,
	_cmp_dp_indr 	= 0xD2,
	_cmp_sr_s_y	= 0xD1,
	_cmp_dp_x_ind	= 0xD1,
	_cmp_dp_y_ind	= 0xD7,
	_cmp_addr_y	= 0xD9,
	_cmp_addr_x	= 0xDD,
	_cmp_long_x	= 0xDF,

	// COP INS
	_cop		= 0x02, //basically a add equivalent

	_dex		= 0xCA, //basically a add(x) equivalent
	_dey		= 0x88, //basically a add(y) equivalent

	// EOR INS
	_eor		= 0x00,

	// INC INS
	_inc_a		= 0x1A, //basically a add equivalent
	_inc_dp		= 0xE6, //basically a add equivalent
	_inc_addr	= 0xEE, //basically a add equivalent
	_inc_dp_x	= 0xF6, //basically a add equivalent
	_inc_addr_x	= 0xFE, //basically a add equivalent
	_inx		= 0xE8, //basically a add(x) equivalent
	_iny		= 0xC8, //basically a add(y) equivalent
	
	// JMP INS
	_jmp		= 0x4C,
	_jmp_addr	= 0x4C,
	_jmp_long	= 0x5C,
	_jmp_addr_abs	= 0x6C,
	_jmp_addr_x	= 0x7C,
	_jmp_addr_long	= 0xDC,
	
	_jsr_addr	= 0x20,
	_jsr_long	= 0x22,
	_jsr_addr_x	= 0xFC,

	_jsr = 0x20,

	_lda 		= 0xAD,
	_ldx 		= 0xAE,
	_ldy 		= 0xAC,
	_lsr		= 0x4E,

	_mvn		= 0x54,
	_mvp		= 0x44,

	// a sleep...?
	_nop		= 0xEA,

	_ora		= 0x0D,
	
	// REP
	_rep		= 0xC2, //reset flag

	// PEA PEI PER PHA PHD PHK PHX PHY PLA PLD PLP PLX PLY INS
	_pea		= 0x00,
	_pei		= 0x00,
	_per		= 0x00,
	_pha		= 0x00,
	_phd		= 0x00,
	_phk		= 0x00,
	_phx		= 0x00,
	_phy		= 0x00,
	_pla		= 0x00,
	_pld		= 0x00,
	_plp		= 0x00,
	_plx		= 0x00,
	_ply		= 0x00,


	_sta_addr 	= 0x8d,
	_sbc 		= 0xe1,
	_stx_addr	= 0x8E,
	_sty_addr	= 0x8C,
	_stp		= 0x00,
	_stz		= 0x00,

	_tax		= 0xAA,
	_tay		= 0xAB,
	_tcd		= 0x5B,
	_tcs		= 0x1B,
	_tdc		= 0x7B,
	_tsc		= 0x3B,
	_tsx		= 0xBA,
	_txa		= 0x8A,
	_txs		= 0x9A,
	_txy		= 0x9B,
	_tya		= 0x98,
	_tyx		= 0xBB,

	_trb		= 0x1C,
	_tsb		= 0x0C,
	
	_wai		= 0xCB,
	_xce		= 0xFB,
};

