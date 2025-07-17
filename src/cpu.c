/* This file is used for CPU operations, like instructions, 
 * it replicates the Ricoh SNES, it's not a hard code, just
 * stuff getting dragged, kinda easy to understand :)    */

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

//This is the ram, probally i will move this to another file in some commit
u32 total_memory[163840] = {};

/* Stack needs to be u8 to compatibility reasons
 * Maybe i change to u32 type to make it more safe
 * to make byte manipulation, but who knows :) */
u8 stack[65536]={};

/* Since 8-bit mode is enabled by default,
 * it's safe to assume his first value */
u32 compare_hex = 0xFF0000;
u32 brk_addr = 0x00FFFE;
u32 cop_addr = 0x00FFF4;

u16 flags_arr[4] = {};

u16 temp_bit;
u16 byte_high;
u16 byte_low;
u32 saved_pc;
u32 address;

u32 a;
u32 pc;

u16 s, x, y;
u16 db, dp, dbr, pb, pbr = 0;
u16 instruction, offset, destination, operation;

u8 z_flag, n_flag, v_flag, m_flag, x_flag, c_flag, d_flag, i_flag, e_flag, b_flag;
u8 p = 0;

int cpu_waiting = 0;

const int dist = 10;
const int rows = 8;
char tot_mem_buffer[sizeof(dist * rows)];
char old_tot_mem_buffer[sizeof(tot_mem_buffer)] = {};

int cycle = 0;

static void execInstruction() {
	//kinda hack but works;	
	if (rom[pc] >= (sizeof(total_memory) / sizeof(total_memory[0]))) {
		printf("ERROR! Address exceed usable memory! \n");
	}
	switch (instruction) {
		//emulator ones
		/*
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
*/
		// actual snes
		case _adc_dp:
			adc_dp();
		break;
		case _adc_l:
			adc_l();
		break;
		case _adc_dp_indr:
			adc_dp_indr();
		break;
		case _adc_dp_indr_l:
			adc_dp_indr_l();
		break;
		case _adc_sr_s_indr_y:
			adc_sr_s_indr_y();
		break;
		case _adc_dp_x_indr:
			adc_dp_x_indr();
		break;
		case _adc_const:
			adc_const();
		break;
		case _adc_addr:
			adc_addr();
		break;
		case _adc_dp_y:
			adc_dp_y();
		break;
		case _adc_dp_x:
			adc_dp_x();
		break;
		case _adc_sr_s:
			adc_sr_s();
		break;
		case _adc_addr_x:
			adc_addr_x();
		break;
		case _adc_addr_y:
			adc_addr_y();
		break;
		case _and_addr:
			and_addr();
		break;
		case _and_l:
			and_l();
		break;
		case _and_addr_y:
			and_addr_y();
		break;
		case _and_addr_x:
			and_addr_x();
		break;
		case _and_dp_indr_l:
			printf("AND DP INDIRECT LONG \n");
		break;
		case _and_dp_indr_y:
			and_dp_indr_y();
		break;
		case _and_dp:
			and_dp();
		break;
		case _and_const:
			and_const();
		break;
		case _and_dp_x:
			and_dp_x();
		break;
		case _and_dp_x_indr:
			and_dp_x_indr();
		break;
		case _asl_dp:
			asl_dp();
		break;
		case _asl_a:
			asl_a();
		break;
		case _asl_addr:
			asl_addr();
		break;
		case _asl_dp_x:
			asl_dp_x();
		break;
		case _asl_addr_x:
			asl_addr_x();
		break;
		case _and_sr_s:
			and_sr_s();
		break;
		case _and_sr_s_indr_y:
			and_sr_s_indr_y();
		break;
		case _bit_dp:
			bit_dp();
		break;
		case _bit_addr:
			bit_addr();
		break;
		case _bit_dp_x:
			bit_dp_x();
		break;
		case _bit_addr_x:
			bit_addr_x();
		break;
		case _bit_const:
			bit_const();
		break;
		case _bcc:
			bcc();
		break;
		case _bcs:
			bcs();
		break;
		case _beq:
			beq();
		break;
		case _bmi:
			bmi();
		break;
		case _bne:
			bne();
		break;
		case _bpl:
			bpl();
		break;
		case _bra:
			bra();
		break;
		case _brl:
			brl();
		break;
		case _brk:
			brk_emu();
		break;
		case _bvc:
			bvc();
		break;
		case _bvs:
			bvs();
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
		case _cmp_const:
			cmp_const();
		break;
		case _cmp_addr:
			cmp_addr();
		break;
		case _cmp_addr_x:
			cmp_addr_x();
		break;
		case _cmp_addr_y:
			cmp_addr_y();
		break;
		case _cmp_l:
			cmp_l();
		break;
		case _cmp_l_x:
			cmp_l_x();
		break;
		case _cmp_dp_x:
			cmp_dp_x();
		break;
		case _cmp_dp_x_indr:
			cmp_dp_x_indr();
		break;
		case _cmp_sr_s:
			cmp_sr_s();
		break;
		case _cmp_dp_indr_y:
			cmp_dp_indr_y();
		break;
		case _cpx_const:
			cpx_const();
		break;
		case _cpx_dp:
			cpx_dp();
		break;
		case _cpx_addr:
			cpx_addr();
		break;
		case _cpy_const:
			cpx_addr();
		break;
		case _cpy_addr:
			cpy_addr();
		break;
		case _dec_a:
			dec_a();
		break;
		case _dec_dp:
			dec_dp();
		break;
		case _dec_addr:
			dec_addr();
		break;
		case _dec_dp_x:
			dec_dp_x();
		break;
		case _dec_addr_x:
			dec_addr_x();
		break;
		case _dex:
			dex_implied();
		break;
		case _dey:
			dey_implied();
		break;
		case _eor_dp_x_indr:
			eor_dp_x_indr();
		break;
		case _eor_sr_s:
			eor_sr_s();
		break;
		case _eor_dp:
			eor_dp();
		break;
		case _eor_dp_x:
			eor_dp_x();
		break;	
		case _eor_addr:
			eor_addr();
		break;
		case _eor_const:
			eor_const();
		break;
		case _eor_addr_y:
			eor_addr_y();
		break;
		case _eor_addr_x:
			eor_addr_x();
		break;
		case _eor_dp_indr_y:
			eor_dp_indr_y();
		break;
		case _eor_dp_indr:
			eor_dp_indr();
		break;
		case _eor_sr_s_indr_y:
			eor_sr_s_indr_y();
		break;
		case _jmp_addr:
			jmp_addr();
		break;
		case _jmp_addr_l:
			jmp_addr_l();
		break;
		case _jmp_addr_x_indr:
			jmp_addr_x_indr();
		break;
		case _jmp_addr_indr_l:
			jmp_addr_indr_l();
		break;
		case _jmp_addr_indr:
			jmp_addr_indr();
		break;
		case _jsr_addr:
			jsr_addr();
		break;
		case _jsr_l:
			jsr_l();
		break;
		case _jsr_addr_x_indr:
			jsr_addr_x_indr();
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
		case _lsr_dp:
			lsr_dp();
		break;
		case _lsr_a:
			lsr_a();
		break;
		case _lsr_addr:
			lsr_addr();
		break;
		case _lsr_dp_x:
			lsr_dp_x();
		break;
		case _lsr_addr_x:
			lsr_addr_x();
		break;
		case _mvn:
			mvn();
		break;
		case _mvp:
			mvp();
		break;
		case _nop:
			nop();
		break;
		case _inc_a:
			inc_a();
		break;
		case _inc_addr:
			inc_addr();
		break;
		case _inc_dp:
			inc_dp();
		break;
		case _inc_addr_x:
			inc_addr_x();
		break;
		case _inc_dp_x:
			inc_dp_x();
		break;
		case _inx:
			inx();
		break;
		case _iny:
			iny();
		break;
		case _lda_addr:
			lda_addr();
		break;
		case _lda_const:
			lda_const();
		break;
		case _lda_addr_y:
			lda_addr_y();
		break;
		case _lda_addr_x:
			lda_addr_x();
		break;
		case _lda_dp:
			lda_dp();
		break;
		case _lda_dp_indr_l:
			lda_dp_indr_l();
		break;
		case _lda_dp_indr_l_y:
			lda_dp_indr_l_y();
		break;
		case _lda_dp_indr_y:
			lda_dp_indr_y();
		break;
		case _lda_l_x:
			lda_l_x();
		break;
		case _lda_l:
			lda_l();
		break;
		case _lda_dp_x:
			lda_dp_x();
		break;
		case _ldx_addr_y:
			ldx_addr_y();
		break;
		case _ldx_addr:
			ldx_addr();
		break;
		case _ldx_const:
			ldx_const();
		break;
		case _ldx_dp:
			ldx_dp();
		break;
		case _ldx_dp_y:
			ldx_dp_y();
		break;
		case _ldy_addr:
			ldy_addr();
		break;
		case _ldy_dp:
			ldy_dp();
		break;
		case _ldy_dp_x:
			ldy_dp_x();
		break;
		case _ldy_const:
			ldy_const();
		break;
		case _ora_l:
			ora_l();
		break;
		case _ora_dp:
			ora_dp();
		break;
		case _ora_dp_indr_y:
			ora_dp_indr_y();
		break;
		case _ora_dp_x_indr:
			ora_dp_x_indr();
		break;
		case _ora_dp_indr:
			ora_dp_indr();
		break;
		case _ora_const:
			ora_const();
		break;
		case _ora_addr:
			ora_addr();
		break;
		case _ora_dp_x:
			ora_dp_x();
		break;	
		case _ora_sr_s:
			ora_sr_s();
		break;
		case _ora_addr_y:
			ora_addr_y();
		break;
		case _ora_addr_x:
			ora_addr_x();
		break;
		case _pla:
			pla();
		break;
		case _plb:
			plb();
		break;
		case _plp:
			plp();
		break;
		case _plx:
			plx();
		break;
		case _ply:
			ply();
		break;
		case _pld:
			pld();
		break;
		case _pha:
			pla();
		break;
		case _phb:
			phb();
		break;
		case _phd:
			phd();
		break;
		case _phx:
			phx();
		break;
		case _phk:
			phk();
		break;
		case _php:
			php();
		break;
		case _phy:
			phy();
		break;
		case _rep:
			rep();
		break;
		case _rol_dp:
			rol_dp();
		break;
		case _rol_a:
			rol_a();
		break;
		case _rol_addr:
			rol_addr();
		break;
		case _rol_dp_x:
			rol_dp_x();
		break;
		case _rol_addr_x:
			rol_addr_x();
		break;
		case _ror_a:
			ror_a();
		break;
		case _ror_addr:
			ror_addr();
		break;
		case _ror_dp:
			ror_dp();
		break;
		case _ror_dp_x:
			ror_dp_x();
		break;
		case _ror_addr_x:
			ror_addr_x();
		break;
		case _rts:
			rts();
		break;
		case _rtl:
			rtl();
		break;
		case _sbc_dp_x_indr:
			sbc_dp_x_indr();
		break;
		case _sbc_sr_s:
			sbc_sr_s();
		break;
		case _sbc_addr:
			sbc_addr();
		break;
		case _sbc_const:
			sbc_const();
		break;
		case _sbc_dp:
			sbc_dp();
		break;
		case _sbc_dp_indr:
			sbc_dp_indr();
		break;
		case _sbc_sr_s_indr_y:
			sbc_sr_s_indr_y();
		break;
		case _sbc_dp_indr_y:
			sbc_dp_indr_y();
		break;
		case _sbc_dp_indr_l_y:
			sbc_dp_indr_l_y();
		break;
		case _sbc_dp_indr_l:
			sbc_dp_indr_l();
		break;
		case _sbc_dp_x:
			sbc_dp_x();
		break;
		case _sbc_addr_y:
			sbc_addr_y();
		break;
		case _sbc_addr_x:
			sbc_addr_x();
		break;
		case _sbc_l:
			sbc_l();
		break;
		case _sbc_l_x:
			sbc_l_x();
		break;
		case _sec:
			sec();
		break;
		case _sed:
			sed();
		break;
		case _sei:
			sei();
		break;
		case _sep:
			sep();
		break;
		case _sta_dp_x:
			sta_dp_x();
		break;
		case _sta_sr_s:
			sta_sr_s();
		break;
		case _sta_dp:
			sta_dp();
		break;
		case _sta_dp_indr_l:
			sta_dp_indr_l();
		break;
		case _sta_dp_indr_l_y:
			sta_dp_indr_l_y();
		break;
		case _sta_addr:
			sta_addr();
		break;
		case _sta_l:
			sta_l();
		break;
		case _sta_dp_indr:
			sta_dp_indr();
		break;
		case _sta_sr_s_y:
			sta_sr_s_y();
		break;
		case _sta_dp_x_indr:
			sta_dp_x_indr();
		break;
		case _sta_dp_indr_y:
			sta_dp_indr_y();
		break;
		case _sta_addr_y:
			sta_addr_y();
		break;
		case _sta_addr_x:
			sta_addr_x();
		break;
		case _sta_l_x:
			sta_l_x();
		break;
		case _stp:
			stp();
		break;
		case _stx_dp:
			stx_dp();
		break;
		case _stx_addr:
			stx_addr();
		break;
		case _stx_dp_y:
			stx_dp_y();
		break;
		case _sty_dp:
			sty_dp();
		break;
		case _sty_addr:
			sty_addr();
		break;
		case _sty_dp_x:
			sty_dp_x();
		break;
		case _stz_dp:
			stz_dp();
		break;
		case _stz_addr:
			stz_addr();
		break;
		case _stz_dp_x:
			stz_dp_x();
		break;
		case _stz_addr_x:
			stz_addr_x();
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
		case _trb_dp:
			trb_dp();
		break;
		case _trb_addr:
			trb_addr();
		break;
		case _tsb_dp:
			tsb_dp();
		break;
		case _tsb_addr:
			tsb_addr();
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
		case _wai:
			wai();
		break;
		case _wdm:
			wdm();
		break;
		case _xba:
			xba();
		break;
		case _xce:
			xce();
		break;
		default:
			printf("Unknown instruction: %X \nCheck CPU instructions header \n", instruction);
		break;
	}
}

extern void printMemory() {
	old_tot_mem_buffer[dist * rows];
	tot_mem_buffer[dist * rows];

	if (strcmp(old_tot_mem_buffer, tot_mem_buffer) != 0) {
		printf(tot_mem_buffer);
	}
	if (strcmp(old_tot_mem_buffer, tot_mem_buffer) == 0) {
		for(int i = 0; i < rows; i += 1) {
			for(int j = 0; j < dist; j += 1) {
				snprintf(tot_mem_buffer, sizeof(tot_mem_buffer), "%i 0x%04X \n", i, total_memory[dist+j]);
				strcpy(old_tot_mem_buffer, tot_mem_buffer);
			}
		}
	}
}
extern void mainFunc() {
	cpu_waiting = 0;
	while (!cpu_waiting) {
		//printMemory();
		#ifdef INTERACTIVE		
		printf("Waiting for instruction...");
		printf("Enter the instruction: ");
		scanf("%x", &instruction);
		#else
		
		instruction = rom[pc++];
		printf("Current: %02X \n", instruction);
		execInstruction();
		printMemory();
		eventLoop();
		// this adda momentum for the instruction for exec (simulate cycles)
		//usleep(cycle);
		#endif
	}
}
