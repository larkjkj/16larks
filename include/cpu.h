#ifndef CPU_HEADER

#include "defs.h"

#define CPU_HEADER

//emulator
extern int cpu_waiting;
extern u16 address;
extern char tot_mem_buffer[200];
extern u16 destination;
extern u16 instruction;

//actual cpu
extern u16 pc; // program counter 

extern i16 a;
extern u16 s;

extern i16 x, y;

extern u16 db;
extern u16 dbr;

extern u16 d;
extern u16 dp;

extern u16 pb;
extern u16 pbr;
extern u8 p;

// separate this shit will be important
extern i8 n_flag;
extern i8 v_flag;
extern i8 m_flag; // 1 or 0 check if it's 8 bit
extern i8 x_flag; // 1 or 0 check if it's 8 bit
extern i8 c_flag;
extern i8 d_flag;
extern i8 z_flag;
extern i8 i_flag;
/*
extern u8 d;
extern u8 i;
extern u8 z;
extern u8 c;
extern u8 e;
extern u8 b;
*/
// n, v, m, x, d, i, z, c, e, b;
// wtf im doing lord

#endif
