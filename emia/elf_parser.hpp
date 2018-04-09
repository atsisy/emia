#pragma once

#include <string>
#include <elf.h>
#include <x86_64-linux-gnu/sys/stat.h>
#include "types.hpp"

class ElfParser {

public:
	/*
	* ELF�w�b�_�̃|�C���^���A�t�@�C�����}�b�v���ꂽ�������̃A�h���X
	* �����Q�̃������A�h���X�͓���ɂȂ邽�ߋ��p�̂Ƃ��Ē�`����B
	*/
	union
	{
		const u8 *mapped_addr;
		Elf64_Ehdr *elf_header;
	};

	/*
	* ELF�ɂ�����v���O�����w�b�_�̃������A�h���X
	*/
	Elf64_Phdr *elf_program_header;

	/*
	* �t�@�C�����
	* stat�V�X�e���R�[���Ŏ擾��������
	*/
	struct stat *file_info;

	/*
	* �R���X�g���N�^
	* ����
	* ELF�`���̎��s�t�@�C���ɑ΂���p�X
	*/
	ElfParser(std::string path);

};