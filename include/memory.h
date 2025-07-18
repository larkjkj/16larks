#ifndef MEMORY_HEADER 
#define MEMORY_HEADER 

#include "defs.h"
#include "arguments.h"

// Stack is a reserved memory
extern u8 stack_memory[65536];

// Ram Memory is the actual RAM
extern u32 ram_memory[65536];

// Total Memory is a simulated 16MB RAM
extern u32 total_memory[163840];
extern u16 temp_bit;

#endif 
