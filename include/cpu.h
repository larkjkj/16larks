#ifndef CPU_HEADER

#include "defs.h"

#define CPU_HEADER

//emulator
extern int cpu_waiting;
extern u16 address;
extern u16 total_memory[65536];
extern char tot_mem_buffer[200];
extern u16 destination;
extern u16 instruction;

//actual cpu
extern u16 pc; // program counter 
extern u16 a;
extern u16 x, y;

extern u16 db;
extern u16 dbr;

extern u16 d;
extern u16 dp;

extern u16 pb;
extern u16 pbr;

// separate this shit will be important
extern u16 p;


// n, v, m, x, d, i, z, c, e, b;
// wtf im doing lord

#endif
