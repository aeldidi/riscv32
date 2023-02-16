#ifndef RISCV32_H
#define RISCV32_H
#include <inttypes.h>
#include <stdbool.h>
#include <stddef.h>

struct riscv32_Cpu {
	uint32_t pc;
	uint32_t registers[32];
};

#define RISCV32_ZERO (0)
#define RISCV32_RA   (1)
#define RISCV32_SP   (2)
#define RISCV32_GP   (3)
#define RISCV32_TP   (4)
#define RISCV32_T0   (5)
#define RISCV32_T1   (6)
#define RISCV32_T2   (7)
#define RISCV32_S0   (8)
#define RISCV32_S1   (9)
#define RISCV32_A0   (10)
#define RISCV32_A1   (11)
#define RISCV32_A2   (12)
#define RISCV32_A3   (13)
#define RISCV32_A4   (14)
#define RISCV32_A5   (15)
#define RISCV32_A6   (16)
#define RISCV32_A7   (17)
#define RISCV32_S2   (18)
#define RISCV32_S3   (19)
#define RISCV32_S4   (20)
#define RISCV32_S5   (21)
#define RISCV32_S6   (22)
#define RISCV32_S7   (23)
#define RISCV32_S8   (24)
#define RISCV32_S9   (25)
#define RISCV32_S10  (26)
#define RISCV32_S11  (27)
#define RISCV32_T3   (28)
#define RISCV32_T4   (29)
#define RISCV32_T5   (30)
#define RISCV32_T6   (31)

void riscv32_execute(struct riscv32_Cpu* cpu, const size_t instructions_len,
		const uint8_t* instructions);
void riscv32_dumpcpu(const struct riscv32_Cpu* cpu);

#endif // RISCV32_H
