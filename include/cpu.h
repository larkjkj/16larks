#ifndef CPU_HEADER
#define CPU_HEADER
#include "defs.h"

/* Btw i will apply some changes to this file, due to compatibility
 * SNES change between 16 and 8 bit addresses, it's great do devs
 * but is horrible to programmers code this for the sake of compatibility :P */


extern int cpu_waiting;

/* Some helpful variables for bitmasking, the names
 * are placeholder, it can be used with context(GREAT
 * IDEA) or without context(A REALLY BAD IDEA)   */

extern u32 address;
extern u16 offset;
extern u16 operation;
extern u16 destination;
extern u16 instruction;
extern u16 byte_high;
extern u16 byte_low;
extern u32 saved_pc;

/* A array which update_flags functions read to...
 * update flags, in the future, i will remove that
 * since using a function just to update each flag
 * it's not a great idea...(why did i did that...) */

extern u16 flags_arr[4];

/* Compare Hex it's just a variable to make
 * things easier to compare, just like
 * COP_ADDR and BRK_ADDR*/

extern u32 compare_hex;
extern u32 cop_addr;
extern u32 brk_addr;

/* WARNING, Accumulator is a 16-bit register
 * but for future reasons i changed to a 32-bit
 * and bitmasked to 24 bit, same happens with pc */

extern u32 a;
extern u32 pc;

/* General Registers */
extern u16 s;

extern u16 x;
extern u16 y;

extern u16 db;
extern u16 dbr;

extern u16 d;
extern u16 dp;

extern u16 pb;
extern u16 pbr;
extern u8 p;

/* Internal SNES Flags, they're changed
 * on almost every single opcode */

extern u8 n_flag;
extern u8 v_flag;
extern u8 m_flag;
extern u8 x_flag;
extern u8 d_flag;
extern u8 i_flag;
extern u8 z_flag;
extern u8 c_flag;
extern u8 e_flag;
extern u8 b_flag;
#endif
