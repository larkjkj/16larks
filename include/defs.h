#include <stdint.h>

#ifndef DEFS_HEADER
#define DEFS_HEADER

typedef uint16_t	u16;
typedef int16_t 	i16;
typedef uint8_t 	u8;
typedef int8_t		i8;

#ifdef _EE
typedef int		i32;
typedef unsigned int	u32;
#else
typedef uint32_t	u32;
typedef int32_t		i32;
#endif 

#endif
