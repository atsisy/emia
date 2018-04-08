#include "types.hpp"
#include "elf_parser.hpp"
#include <x86_64-linux-gnu/sys/mman.h>
#include <x86_64-linux-gnu/sys/types.h>
#include <x86_64-linux-gnu/sys/unistd.h>
#include <x86_64-linux-gnu/sys/stat.h>
#include <x86_64-linux-gnu/sys/fcntl.h>
#include <iostream>

ElfParser::ElfParser(std::string path)
{
	file_info = new struct stat;

	int fd = open(path.c_str(), O_RDONLY);
	if (fd == -1) {
		std::cerr << path << ": File not found." << std::endl;
		exit(-1);
	}

	if (fstat(fd, file_info) == -1) {
		std::cerr << "Failed: fstat" << std::endl;
		exit(-1);
	}
	u8 *base_addr = (u8 *)mmap(NULL, file_info->st_size, PROT_READ, MAP_PRIVATE, fd, 0);

	this->elf_header = (Elf64_Ehdr *)base_addr;
	this->elf_program_header = (Elf64_Phdr *)(base_addr + sizeof(Elf64_Ehdr));

	close(fd);
}