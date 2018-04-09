#pragma once

#include "register.hpp"
#include "elf_parser.hpp"

class Emulator {
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

	~Emulator();
};