#include <stdio.h>
#include <string.h>

#include "arguments.h"
#include "cpu.h"
#include "rom_funcs.h"

int norom = 0;
int verbose = 0;

int main(int argc, char* argv[]) {
	for(int i = 0; i < argc; i += 1) {
		if (strcmp(argv[i], "--verbose") == 0) {
			verbose = 1;
		}
		if (strcmp(argv[i], "--norom") == 0) {
			norom = 1;
		}
	}
	identifyRom();
	//splitRom(argv[1]);
	printf("Verbose is %d \n", verbose);
	printf("NoRom is %d \n", norom);
	while (1) {
		//printMemory();
		if (mainFunc() > 0) {
			printf("Return code is not 0, something is wrong \n");
		}
		break;
	}
	return 0;
}
