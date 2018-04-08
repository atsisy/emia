#include "emulator.hpp"
#include "elf_parser.hpp"
#include <cstring>

Emulator::Emulator(u64 mem_size, ElfParser & parser, u64 rsp)
	: registers(parser.elf_header->e_entry)
{
	memory = new u8[mem_size];
	memcpy(memory + 0x400000, parser.mapped_addr, parser.file_info->st_size);
}

Emulator::~Emulator()
{
	delete memory;
}