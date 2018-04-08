#include <iostream>
#include "elf_parser.hpp"
#include "emulator.hpp"

int main(int argc, char **)
{
	ElfParser parser("/home/debian/projects/emia/a.out");

	Emulator *emu = new Emulator(
		0xffffff,
		parser,
		0xffffff
	);

	printf("ditic: %x\n", emu->memory[emu->registers.rip]);
	return 0;
}