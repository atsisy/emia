#include <cstdio>
#include <cstring>
#include "register.hpp"

Registers::Registers(u64 rip)
{
	memset(this->gp_registers, 0, sizeof(this->gp_registers));
	this->rip = rip;
}

Registers::~Registers()
{}

u64 Registers::read_register64(u16 register_id)
{
	return gp_registers[register_id];
}

void RflagsRegister::setup_sf()
{
	sf = 1;
}

void RflagsRegister::setup_of()
{
	of = 1;
}

void RflagsRegister::setup_zf()
{
	zf = 1;
}

void RflagsRegister::print_status()
{
	printf("Rflags: 0x%016lx\n", rflags);
}

u64 *Registers::ref_register64(u16 register_id)
{
	return gp_registers + (register_id << 3);
}

void Registers::print_status()
{
	for (size_t i = 0; i < GPR_COUNT; ++i) {
		printf("R%d: 0x%016lx\n", i, read_register64(i));
	}

	rflags.print_status();
}

ModR_M::ModR_M(u8 byte)
{
	modr_m = byte;
}