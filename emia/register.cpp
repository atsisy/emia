#include <cstdio>
#include <cstring>
#include "register.hpp"

Registers::Registers(u64 rip)
{
	memset(this->gp_registers, 0, sizeof(this->gp_registers));
	this->rip = rip + 0x400000;
}

Registers::~Registers()
{}

u64 Registers::read_register64(u16 register_id)
{
	return gp_registers[register_id];
}


u64 *Registers::ref_register64(u16 register_id)
{
	return gp_registers + (register_id << 3);
}

void Registers::print_status()
{
	for (size_t i = 0; i < GPR_COUNT; ++i) {
		printf("R%d: 0x%x\n", i, read_register64(i));
	}
}