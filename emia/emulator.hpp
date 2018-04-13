#pragma once

#include <unordered_map>
#include <functional>
#include "register.hpp"
#include "elf_parser.hpp"

class Emulator;

class DirectiveTable {

protected:
	std::unordered_map<u64, std::function<u64(Emulator *)>> directives;

	static u64 mov_r64_imm64(Emulator *emulator);
	static u64 jmp_rel8(Emulator *emulator);

	static u64 add_rm64_imm8(Emulator *emulator);

public:
	DirectiveTable();
	~DirectiveTable();
};

class Emulator : public DirectiveTable {

	friend DirectiveTable;

private:
	u8 get_code8(u64 byte_offset);
	u16 get_code16(u64 byte_offset);
	u32 get_code32(u64 byte_offset);
	u64 get_code64(u64 byte_offset);

	u64 change_rip(i64 offset);
	bool is_prefix(u8 code);
	u8 *next_directive();

	u64 get_opecode(u8 *top_of_memory);

public:
	/*
	* ���W�X�^
	*/
	Registers registers;

	/*
	* ������
	*/
	u8 *memory;

	/*
	* �R���X�g���N�^
	* ����
	* u64 mem_size: �m�ۂ��郁�����̃T�C�Y
	* ElfParser & parser: ELF���s�t�@�C������͂���ELF�p�[�T
	* u64 rsp: �X�^�b�N�|�C���^�̏����l���w�肷��
	*/
	Emulator(u64 mem_size, ElfParser & parser, u64 rsp);

	void execution_loop();

	~Emulator();
};