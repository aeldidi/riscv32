#include <errno.h>
#include <stdarg.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "riscv32.h"

#include "panic.c"

uint8_t*
readfull(FILE* f, size_t* out_len)
{
	uint8_t* result = calloc(2, 1);
	if (result == NULL) {
		return result;
	}

	int    c = 0;
	size_t i = 0;
	for (;;) {
		c = fgetc(f);
		if (c == EOF) {
			break;
		}
		result[i] = c & 0xff;
		i += 1;
		void* ptr = realloc(result, i + 2);
		if (ptr == NULL) {
			free(result);
			return NULL;
		}
		result = ptr;
	}

	*out_len = i;
	return result;
}

int
main(int argc, char** argv)
{
	if (argc < 2) {
		fprintf(stderr, "usage: %s path/to/executable", argv[0]);
		return EXIT_FAILURE;
	}

	FILE* f = fopen(argv[1], "r");
	if (f == NULL) {
		panic("error opening %s: %s", argv[1], strerror(errno));
	}

	size_t   executable_len = 0;
	uint8_t* executable     = readfull(f, &executable_len);
	fclose(f);

	if (executable_len % 4 != 0) {
		panic("bruh moment");
	}

	struct riscv32_Cpu cpu = {0};
	riscv32_execute(&cpu, executable_len, executable);

	printf("execution completed\n");
	printf("registers:\n");
	riscv32_dumpcpu(&cpu);
}
