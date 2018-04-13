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
		ret |= ((u64)get_code8(byte_offset + i)) << (i << 3);
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

	directives.insert(std::make_pair(0xeb, jmp_rel8));
	directives.insert(std::make_pair(0x4883, add_rm64_imm8));
}

DirectiveTable::~DirectiveTable()
{
}

u64 DirectiveTable::mov_r64_imm64(Emulator *emulator)
{
	/*
	* 最初のバイトに含まれるREXプレフィクスは飛ばす
	* 64bit拡張であることはわかっている
	*/

	/*
	* オペコード読み取り
	* 仕様では0xb8 + rdとなっている
	*/
	u8 register_id = emulator->get_code8(1) - 0xb8;

	/*
	* 即値読み取り
	* オペコードの後ろに64bitの即値がリトルエンディアンで格納されている
	*/
	u64 imm64 = emulator->get_code64(2);

	// 値代入
	*emulator->registers.ref_register64(register_id) = imm64;

	// 1 + 1 + 8 byte
	emulator->change_rip(10);

	/*
	* mov命令はrflagsを変化させない
	*/
	return emulator->registers.rflags.rflags;
}

u64 DirectiveTable::jmp_rel8(Emulator *emulator)
{
	constexpr u8 jmp_rel8_size = 2;

	i8 rel8 = emulator->get_code8(1) + jmp_rel8_size;
	emulator->registers.rip += rel8;
	return emulator->registers.rflags.rflags;
}

u64 DirectiveTable::add_rm64_imm8(Emulator *emulator)
{
	/*
	* ModR_Mバイトを取得
	*/
	ModR_M modr_m(emulator->get_code8(2));

	printf("MODRM -> %x\n", modr_m.modr_m);

	/*
	* 関連してくるレジスタへのポインタを取得（疑似。実際そんなもんはない）
	*/
	u64 *target_register = emulator->registers.ref_register64(modr_m.r_m);
	i8 imm8 = emulator->get_code8(3);

	printf("imm8 -> %d\n", imm8);
	printf("MOD -> %d\n", modr_m.mod);

	switch (modr_m.mod) {
	case 0b000:
		/*
		* [register] <- add imm8
		*/
		*(u8 *)(*target_register) = imm8;
		break;
	case 0b0011:
		/*
		* register <- add imm8
		*/
		*target_register += imm8;
		break;
	default:
		break;
	}

	// prefix + opecode + ModR_M + imm8
	emulator->change_rip(4);
}

u8 *Emulator::next_directive()
{
	return (u8 *)(memory + registers.rip);
}

bool Emulator::is_prefix(u8 code)
{
	/*
	* REX prefixの上位4bitは0100で固定なのでここからprefixかを判定する
	*/
	return 0b0100 == (code >> 4);
}

u64 Emulator::get_opecode(u8 *top_of_memory)
{
	u64 code = 0;
	u8 tmp_byte, count = 0;

	top_of_memory = &memory[registers.rip];

	do {
		/*
		* 命令取り出し
		*/
		tmp_byte = *top_of_memory++;

		/*
		* byteをcount * 8だけシフトしてorする
		*/
		code = (code << 8) | tmp_byte;

		printf("opecoding -> %x\n", code);

		if (!is_prefix(tmp_byte)) {
			/*
			* バイトコードがprefixでなかった場合、
			* オペコードをcodeにorしてbreak
			*/
			break;
		}
	} while (++count);

	return code;
}

void Emulator::execution_loop()
{
	u64 code, opecode;
	u8 *top_memory;

	do {
		registers.print_status();

		code = 0;
		top_memory = next_directive();
		opecode = get_opecode(top_memory);

		printf("opecode -> %x\n", opecode);

		directives.at(opecode)(this);
	}while(1);
}