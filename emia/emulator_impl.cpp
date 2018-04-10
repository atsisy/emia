#include "emulator.hpp"
#include "elf_parser.hpp"
#include <cstring>

Emulator::Emulator(u64 mem_size, ElfParser & parser, u64 rsp)
	: registers(parser.elf_header->e_entry)
{
	memory = new u8[mem_size];

	/*
	* メモリ上にマップされた実行ファイルを
	*/
	memcpy(memory + 0x400000, parser.mapped_addr, parser.file_info->st_size);
}

Emulator::~Emulator()
{
	delete memory;
}

u8 Emulator::get_code8(u64 byte_offset)
{
	return memory[registers.rip + byte_offset];
}

u16 Emulator::get_code16(u64 byte_offset)
{
	u16 ret = 0;

	/*
	* リトルエンディアンで格納されているため、それを整数に戻す
	*/
	for (size_t i = 0; i < 2; ++i) {
		ret |= get_code8(byte_offset + i) << (i << 3);
	}

	return ret;
}

u32 Emulator::get_code32(u64 byte_offset)
{
	u32 ret = 0;

	/*
	* リトルエンディアンで格納されているため、それを整数に戻す
	*/
	for (size_t i = 0; i < 4; ++i) {
		ret |= get_code8(byte_offset + i) << (i << 3);
	}

	return ret;
}

u64 Emulator::change_rip(i64 offset)
{
	/*
	* 新しいrip値を演算し、返却する
	*/
	registers.rip += offset;
	return registers.rip;
}

u64 Emulator::get_code64(u64 byte_offset)
{
	u64 ret = 0;

	/*
	* リトルエンディアンで格納されているため、それを整数に戻す
	*/
	for (size_t i = 0; i < 8; ++i) {
		ret |= get_code8(byte_offset + i) << (i << 3);
	}

	return ret;
}

DirectiveTable::DirectiveTable()
	: directives()
{
	size_t i;

	/*
	* r0 ~ r15までのmov r64, imm64についての命令を格納
	*/
	for (i = 0; i < 16; ++i) {
		directives.insert(std::make_pair((0x48 << 8) | (0xb8 + i), mov_r64_imm64));
	}
}

DirectiveTable::~DirectiveTable()
{
}

u64 DirectiveTable::mov_r64_imm64(Emulator &emulator)
{
	/*
	* 最初のバイトに含まれるREXプレフィクスは飛ばす
	* 64bit拡張であることはわかっている
	*/

	/*
	* オペコード読み取り
	* 仕様では0xb8 + rdとなっている
	*/
	u8 register_id = emulator.get_code8(1) - 0xb8;

	/*
	* 即値読み取り
	* オペコードの後ろに64bitの即値がリトルエンディアンで格納されている
	*/
	u64 imm64 = emulator.get_code64(2);

	// 値代入
	*emulator.registers.ref_register64(register_id) = imm64;

	// 1 + 1 + 8 byte
	emulator.change_rip(10);
}

void Emulator::execution_loop()
{

}