#include <inttypes.h>
#include <stdarg.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>

#include "riscv32.h"

#include "panic.c"

enum riscv32_instructions {
	ADDI,
	LUI,
	ECALL,
	UNKNOWN,
};

enum riscv32_instructions
decode(const uint32_t instruction)
{
	uint32_t mask = 0b00000000000000000000000001111111;
	if ((instruction & mask) == 0b00000000000000000000000000110111) {
		return LUI;
	}

	if ((instruction & mask) == 0x73) {
		return ECALL;
	}

	if ((instruction & 0b00000000000000000111000001111111) ==
			0b00000000000000000000000000010011) {
		return ADDI;
	}

	return UNKNOWN;
}

void
riscv32_execute(struct riscv32_Cpu* cpu, const size_t instructions_len,
		const uint32_t* instructions)
{
	while (true) {
		if (cpu->pc >= instructions_len) {
			panic("out of bounds pc: %zu", cpu->pc);
		}
		uint32_t instruction = instructions[cpu->pc];
		uint32_t opcode      = decode(instruction);
		switch (opcode) {
		case ECALL:
			// FIXME: The ecall instruction is used to exit,
			//        regardless of what's in a7.
			return;
		case LUI: {
			uint32_t reg = (instruction >> 7) &
				       0b00000000000000000000000000011111;
			uint32_t imm        = (instruction >> 12) << 12;
			cpu->registers[reg] = imm;
			cpu->pc += 1;
			break;
		}
		case ADDI: {
			uint32_t dest = (instruction & 0b00000000000000000000111110000000) >>
					7;
			uint32_t src = (instruction & 0b00000000000011111000000000000000) >>
				       15;
			uint32_t imm         = instruction >> 20;
			cpu->registers[dest] = cpu->registers[src] + imm;
			cpu->pc += 1;
			break;
		}
		default:
			panic("unimplemented opcode: %" PRIu32 "", opcode);
		}
	}
}

void
riscv32_dumpcpu(const struct riscv32_Cpu* cpu)
{
	printf("ra:  %" PRId32 "\n", cpu->registers[RISCV32_RA]);
	printf("sp:  %" PRId32 "\n", cpu->registers[RISCV32_SP]);
	printf("gp:  %" PRId32 "\n", cpu->registers[RISCV32_GP]);
	printf("tp:  %" PRId32 "\n", cpu->registers[RISCV32_TP]);
	printf("t0:  %" PRId32 "\n", cpu->registers[RISCV32_T0]);
	printf("t1:  %" PRId32 "\n", cpu->registers[RISCV32_T1]);
	printf("t2:  %" PRId32 "\n", cpu->registers[RISCV32_T2]);
	printf("s0:  %" PRId32 "\n", cpu->registers[RISCV32_S0]);
	printf("s1:  %" PRId32 "\n", cpu->registers[RISCV32_S1]);
	printf("a0:  %" PRId32 "\n", cpu->registers[RISCV32_A0]);
	printf("a1:  %" PRId32 "\n", cpu->registers[RISCV32_A1]);
	printf("a2:  %" PRId32 "\n", cpu->registers[RISCV32_A2]);
	printf("a3:  %" PRId32 "\n", cpu->registers[RISCV32_A3]);
	printf("a4:  %" PRId32 "\n", cpu->registers[RISCV32_A4]);
	printf("a5:  %" PRId32 "\n", cpu->registers[RISCV32_A5]);
	printf("a6:  %" PRId32 "\n", cpu->registers[RISCV32_A6]);
	printf("a7:  %" PRId32 "\n", cpu->registers[RISCV32_A7]);
	printf("s0:  %" PRId32 "\n", cpu->registers[RISCV32_S0]);
	printf("s1:  %" PRId32 "\n", cpu->registers[RISCV32_S1]);
	printf("s2:  %" PRId32 "\n", cpu->registers[RISCV32_S2]);
	printf("s3:  %" PRId32 "\n", cpu->registers[RISCV32_S3]);
	printf("s4:  %" PRId32 "\n", cpu->registers[RISCV32_S4]);
	printf("s5:  %" PRId32 "\n", cpu->registers[RISCV32_S5]);
	printf("s6:  %" PRId32 "\n", cpu->registers[RISCV32_S6]);
	printf("s7:  %" PRId32 "\n", cpu->registers[RISCV32_S7]);
	printf("s8:  %" PRId32 "\n", cpu->registers[RISCV32_S8]);
	printf("s9:  %" PRId32 "\n", cpu->registers[RISCV32_S9]);
	printf("s10: %" PRId32 "\n", cpu->registers[RISCV32_S10]);
	printf("s11: %" PRId32 "\n", cpu->registers[RISCV32_S11]);
	printf("t3:  %" PRId32 "\n", cpu->registers[RISCV32_T3]);
	printf("t4:  %" PRId32 "\n", cpu->registers[RISCV32_T4]);
	printf("t5:  %" PRId32 "\n", cpu->registers[RISCV32_T5]);
	printf("t6:  %" PRId32 "\n", cpu->registers[RISCV32_T6]);
	printf("pc:  %" PRId32 "\n", cpu->pc);
}