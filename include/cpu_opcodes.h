#include <stdio.h>
#include <stdlib.h>

#include "defs.h"

extern void update_n_flag(int count, u16 operations[]); 
extern void update_x_flag(u16 operation);
extern void update_z_flag(int count, u16 operations[]); 
extern void update_c_flag(u16 operations[]); 
extern void update_flags(u16 operation); 
extern int mergeBytes();

// Emulator Ones
extern void add();
extern void mov();
extern void prn();
extern void jmp();
extern void ato();

extern void adc_dp_x();
extern void adc_sr_s();
extern void adc_dp();
extern void adc_dp_indr_l();
extern void adc_dp_indr();
extern void adc_dp_indr_x();
extern void adc_const();
extern void adc_addr();
extern void adc_l();
extern void adc_dp_y();
extern void adc_dp();
extern void adc_sr_s_indr_y();
extern void adc_dp_x();
extern void adc_dp_y();
extern void adc_addr_y();
extern void adc_addr_x();
extern void adc_long_x();

extern void asl_addr();
extern void asl_a();
extern void asl_dp();
extern void asl_dp_x();
extern void asl_addr_x();

extern void and_addr();
extern void and_dp();
extern void and_dp_y();
extern void and_dp_x();
extern void and_addr_y();
extern void and_addr_x();
extern void and_const();
extern void and_sr_s();
extern void and_sr_s_indr_y();

extern void bit_dp();
extern void bit_dp_x();
extern void bit_addr();
extern void bit_addr_x();
extern void bit_const();

extern void bcc();
extern void bcs();
extern void beq();
extern void bmi();
extern void bne();
extern void bpl();
extern void bra();
extern void brl();
extern void bvc();
extern void bvs();
extern void brk_emu();

extern void clc();
extern void cld();
extern void cli();
extern void clv();

extern void cmp_dp_x_indr();
extern void cmp_sr_s();
extern void cmp_const();
extern void cmp_addr();
extern void cmp_dp_x();
extern void cmp_dp_indr_y();
extern void cmp_addr_y();
extern void cmp_addr_x();
extern void cmp_l();
extern void cmp_l_x();

extern void cpy_addr();
extern void cpy_const();
extern void cpx_const();
extern void cpx_dp();
extern void cpx_addr();

extern void dec_a();
extern void dec_dp();
extern void dec_addr();
extern void dec_dp_x();
extern void dec_addr_x();

extern void dex_implied();
extern void dey_implied();

extern void eor_dp_x_indr();
extern void eor_sr_s();
extern void eor_dp();
extern void eor_dp_indr();
extern void eor_sr_s_indr_y();
extern void eor_dp_x();
extern void eor_addr();
extern void eor_const();
extern void eor_addr_y();
extern void eor_addr_x();
extern void eor_dp_indr_y();
extern void eor_dp_indr_x();

extern void mvn();
extern void mvp();
extern void nop();

extern void inc_a();
extern void inc_dp();
extern void inc_addr();
extern void inc_dp_x();
extern void inc_addr_x();

extern void inx();
extern void iny();

extern void jsr_addr();
extern void jsr_long();
extern void jsr_addr_x_indr();

extern void jmp_addr();
extern void jmp_addr_indr();
extern void jmp_addr_x_indr();
extern void jmp_addr_l();
extern void jmp_addr_indr_l();

extern void lda_addr();
extern void lda_const();
extern void lda_addr_x();
extern void lda_addr_y();
extern void lda_dp();
extern void lda_l_x();
extern void lda_dp_indr_l();
extern void lda_dp_indr_y();
extern void lda_dp_x();
extern void lda_dp_y();
extern void ldx_addr();
extern void ldx_addr_y();
extern void ldx_dp();
extern void ldx_dp_y();
extern void ldy_addr();
extern void ldy_const();
extern void ldy_dp();
extern void ldy_dp_x();

extern void lsr_dp();
extern void lsr_a();
extern void lsr_addr();
extern void lsr_dp_x();
extern void lsr_addr_x();

extern void ora_dp();
extern void ora_dp_indr_y();
extern void ora_dp_x_indr();
extern void ora_dp_x();
extern void ora_sr_s();
extern void ora_const();
extern void ora_addr();
extern void ora_addr_y();
extern void ora_addr_x();

extern void pld();
extern void pha();
extern void phb();
extern void phb();
extern void phd();
extern void phk();
extern void php();
extern void phx();
extern void phy();
extern void pla();
extern void plb();
extern void pld();
extern void plp();
extern void plx();
extern void ply();

extern void rol_dp();
extern void rol_a();
extern void rol_addr();
extern void rol_dp_x();
extern void rol_addr_x();

extern void ror_dp();
extern void ror_a();
extern void ror_addr();
extern void ror_dp_x();
extern void ror_addr_x();

extern void rts();
extern void rep();

extern void sbc_addr();
extern void sbc_const();
extern void sbc_dp();
extern void sbc_dp_x();
extern void sbc_sr_s();
extern void sbc_sr_s_indr_y();
extern void sbc_dp_indr();
extern void sbc_dp_indr_y();
extern void sbc_dp_indr_l_y();
extern void sbc_dp_x_indr();
extern void sbc_dp_indr_l();
extern void sbc_long_y();
extern void sbc_l();
extern void sbc_addr_y();
extern void sbc_addr_x();
extern void sbc_l_x();

extern void sec();
extern void sed();
extern void sei();
extern void sep();

extern void sta_dp_x();
extern void sta_sr_s();
extern void sta_dp();
extern void sta_dp_l();
extern void sta_addr();
extern void sta_l();
extern void sta_dp_y();
extern void sta_dp_indr();
extern void sta_dp_indr_l();
extern void sta_dp_indr_l_y();
extern void sta_dp_x_indr();
extern void sta_dp_indr_y();
extern void sta_sr_s_y();
extern void sta_dp_y();
extern void sta_addr_y();
extern void sta_addr_x();
extern void sta_l_x();

extern void stp();

extern void stx_addr();
extern void stx_dp();
extern void stx_dp_y();
extern void stx_addr();
extern void sty_dp();
extern void sty_addr();
extern void sty_dp_x();
extern void stz_dp();
extern void stz_dp_x();
extern void stz_addr();
extern void stz_addr_x();

extern void txs();
extern void tsc();
extern void tsx();
extern void txy();
extern void tyx();
extern void tdc();
extern void tya();
extern void txa();
extern void tax();
extern void tay();
extern void tcd();
extern void tcs();
extern void tdc();
extern void trb_dp();
extern void trb_addr();
extern void tsb_dp();
extern void tsb_addr();
extern void wai();
extern void wdm();

extern void xce();
extern void xba();
