#include "emulator.hpp"
#include "elf_parser.hpp"
#include <cstring>

Emulator::Emulator(u64 mem_size, ElfParser & parser, u64 rsp)
	: registers(parser.elf_header->e_entry)
{
	memory = new u8[mem_size];

	/*
	* ��������Ƀ}�b�v���ꂽ���s�t�@�C����
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
	* ���g���G���f�B�A���Ŋi�[����Ă��邽�߁A����𐮐��ɖ߂�
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
	* ���g���G���f�B�A���Ŋi�[����Ă��邽�߁A����𐮐��ɖ߂�
	*/
	for (size_t i = 0; i < 4; ++i) {
		ret |= get_code8(byte_offset + i) << (i << 3);
	}

	return ret;
}

u64 Emulator::change_rip(i64 offset)
{
	/*
	* �V����rip�l�����Z���A�ԋp����
	*/
	registers.rip += offset;
	return registers.rip;
}

u64 Emulator::get_code64(u64 byte_offset)
{
	u64 ret = 0;

	/*
	* ���g���G���f�B�A���Ŋi�[����Ă��邽�߁A����𐮐��ɖ߂�
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
	* r0 ~ r15�܂ł�mov r64, imm64�ɂ��Ă̖��߂��i�[
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
	* �ŏ��̃o�C�g�Ɋ܂܂��REX�v���t�B�N�X�͔�΂�
	* 64bit�g���ł��邱�Ƃ͂킩���Ă���
	*/

	/*
	* �I�y�R�[�h�ǂݎ��
	* �d�l�ł�0xb8 + rd�ƂȂ��Ă���
	*/
	u8 register_id = emulator.get_code8(1) - 0xb8;

	/*
	* ���l�ǂݎ��
	* �I�y�R�[�h�̌���64bit�̑��l�����g���G���f�B�A���Ŋi�[����Ă���
	*/
	u64 imm64 = emulator.get_code64(2);

	// �l���
	*emulator.registers.ref_register64(register_id) = imm64;

	// 1 + 1 + 8 byte
	emulator.change_rip(10);
}

void Emulator::execution_loop()
{

}