#ifndef CPU_HEADER

/* Btw i will apply some changes to this file, due to compatibility
 * SNES change between 16 and 8 bit addresses, it's great do devs
 * but is horrible to programmers code this for the sake of compatibility :P */

#define CPU_HEADER
#include "defs.h"

//emulator
extern int cpu_waiting;
extern u32 address;
extern u16 offset;
extern u16 operation;
extern u16 destination;
extern u16 instruction;
extern u16 byte_high;
extern u16 byte_low;
extern u32 saved_pc;
extern u16 flags_arr[10];
extern u32 compare_hex;

//actual cpu
extern u32 pc; // program counter 

/* WARNING, Accumulator is a 16-bit register
 * but for future reasons i changed to a 32-bit
 * and bitmasked it(it makes sense since 24-bit
 * operations can't be made with 16-bit due to
 * these 8-bit difference); a temp_a cast can be
 * made but it costs a operation(decreases speed)*/
extern u32 a;

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

// separate this shit will be important
//Cant make them bool, since they are used on bitwise
extern u8 n_flag;
extern u8 v_flag;
extern u8 m_flag; // 1 or 0 check if it's 8 bit
extern u8 x_flag; // 1 or 0 check if it's 8 bit
extern u8 d_flag;
extern u8 i_flag;
extern u8 z_flag;
extern u8 c_flag;

extern u8 e_flag;
extern u8 b_flag;
// n, v, m, x, d, i, z, c, e, b;
// wtf im doing lord

#endif
