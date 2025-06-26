#include <stdio.h>
#include "cpu.h"
#include "rom.h"

int main(int argc, char* argv[]) {
	if (argv[1] != NULL) {
		splitRom(argv[1]);
	}
	while (1) {
		//printMemory();
		if (mainFunc() > 0) {
			printf("Return code is not 0, something is wrong \n");
		}
		break;
	}
	return 0;
}
