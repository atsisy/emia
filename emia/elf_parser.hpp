#pragma once

#include <string>
#include <elf.h>
#include <x86_64-linux-gnu/sys/stat.h>
#include "types.hpp"

class ElfParser {

public:
	union
	{
		const u8 *mapped_addr;
		Elf64_Ehdr *elf_header;
	};
	Elf64_Phdr *elf_program_header;

	struct stat *file_info;

	ElfParser(std::string path);

};