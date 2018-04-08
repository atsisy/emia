#pragma once

#include "register.hpp"
#include "elf_parser.hpp"

class Emulator {
public:
	Registers registers;
	u8 *memory;

	Emulator(u64 mem_size, ElfParser & parser, u64 rsp);
	~Emulator();
};