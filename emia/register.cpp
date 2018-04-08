#include <cstring>
#include "register.hpp"

Registers::Registers(u64 rip)
{
	memset(this->registers, 0, sizeof(this->registers));
	this->rip = rip + 0x400000;
}

Registers::~Registers()
{}

u64 Registers::read_register64(u16 register_id)
{
	return registers[register_id];
}


u64 *Registers::ref_register64(u16 register_id)
{
	return registers + (register_id << 3);
}