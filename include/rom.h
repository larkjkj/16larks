extern void splitRom(const char* rom);

struct rom {
	char*	optional_header;	// 512byte header
	char*	rom_address;		// total addresses
	char*	start_addr;		// sfc or smc indepedent code
	char	type; 			// LoRom or HiRom
} rom_info;
