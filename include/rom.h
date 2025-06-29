#include "defs.h"
#include "instructions.h"

extern u16 fake_rom[];
extern u16 rom[];

u16 fake_rom[] = {
	_mov, 0x0012, 0x00FF, _mov, 0x00A0, 0x0042,
	_mov, 0x00C1, 0x0F0F, _mov, 0x0030, 0x1234,
	_mov, 0x0100, 0xAAAA, _add, 0x0012, _add,
	0x00A0, _add, 0x00A0, _add, 0x0030, _add,
	0x0100, _prn, 0x0010, 0x0015, _prn, 0x00A0,
	0x00A3, _prn, 0x00C0, 0x00C3, _mov, 0x0020,
	0x0021, _mov, 0x0021, 0x0022, _mov, 0x0022,
	0x0023, _mov, 0x0023, 0x0024, _mov, 0x0024,
	0x0025, _add, 0x0020, _add, 0x0021, _add,
	0x0022, _add, 0x0023, _add, 0x0024, _prn,
	0x0020, 0x0026, _mov, 0x00B5, 0x7777, _add,
	0x00B5, _add, 0x00B5, _prn, 0x00B0, 0x00B8,
	_mov, 0x01F0, 0xDEAD, _mov, 0x01F1, 0xBEEF,
	_mov, 0x01F2, 0xC0DE, _add, 0x01F2, _add,
	0x01F2, _prn, 0x01F0, 0x01F3, _mov, 0x0001,
	0x9999, _add, 0x0001, _add, 0x0001, _prn,
	0x0000, 0x0003, _mov, 0x0030, 0xFFFF, _add,
	0x0030, _add, 0x0030, _prn, 0x0030, 0x0031,
	_eoq
};

u16 rom[] = {};
