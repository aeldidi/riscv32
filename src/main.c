#include <errno.h>
#include <stdarg.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "riscv32.h"

void
panic_impl(const char* filename, const int line, const char* fmt, ...)
{
	va_list args;
	va_start(args, fmt);
	fprintf(stderr, "panic:%s:%d ", filename, line);
	vfprintf(stderr, fmt, args);
	fprintf(stderr, "\n");
	va_end(args);

	exit(EXIT_FAILURE);
}
#define panic(...) panic_impl(__FILE__, __LINE__, __VA_ARGS__)
#define unimplemented()                                                       \
	panic_impl(__FILE__, __LINE__, "reached unimplemented code")
#define unreachable()                                                         \
	panic_impl(__FILE__, __LINE__, "reached unreachable code")

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

enum riscv32_instructions {
	LUI,
	ECALL,
	UNKNOWN,
};

enum riscv32_instructions
decode(const uint32_t instruction)
{
	uint32_t mask = 0b00000000000000000000000001111111;

	switch (instruction & mask) {
	case 0x73:
		return ECALL;
	case 0b00000000000000000000000000110111:
		return LUI;
	default:
		return UNKNOWN;
	}
}

void
riscv32_execute(struct riscv32_Cpu* cpu, const size_t instructions_len,
		const uint32_t* instructions)
{
	// FIXME: The ecall instruction is used to exit, regardless of what's
	//        in a7.
	while (true) {
		if (cpu->pc >= instructions_len) {
			panic("out of bounds pc: %zu", cpu->pc);
		}
		uint32_t instruction = instructions[cpu->pc];
		uint32_t opcode      = decode(instruction);
		switch (opcode) {
		case ECALL:
			return;
		case LUI: {
			uint32_t reg = (instruction >> 7) &
				       0b00000000000000000000000000011111;
			uint32_t imm        = (instruction >> 12) << 12;
			cpu->registers[reg] = imm;
			cpu->pc += 1;
			break;
		}
		default:
			panic("unimplemented opcode: %" PRIu32 "", opcode);
		}
	}
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

	size_t    executable_len = 0;
	uint32_t* executable     = (uint32_t*)readfull(f, &executable_len);
	fclose(f);

	if (executable_len % 4 != 0) {
		panic("bruh moment");
	}
	executable_len /= 4;

	struct riscv32_Cpu cpu = {0};
	riscv32_execute(&cpu, executable_len, executable);

	printf("execution completed\n");
	for (size_t i = 0; i < 32; i += 1) {
		printf("register %.2zu: %" PRId32 "\n", i, cpu.registers[i]);
	}
}
