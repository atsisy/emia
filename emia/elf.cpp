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
	/*
	* �t�@�C�����󂯎��̂��߂̍\���̂�������
	*/
	file_info = new struct stat;

	// �t�@�C���I�[�v���B�G���[�Ȃ烁�b�Z�[�W�o���ďI��
	int fd = open(path.c_str(), O_RDONLY);
	if (fd == -1) {
		std::cerr << path << ": File not found." << std::endl;
		exit(-1);
	}

	/*
	* �J�����t�@�C���̏����擾
	* �G���[�������烁�b�Z�[�W�o���ďI��
	*/
	if (fstat(fd, file_info) == -1) {
		std::cerr << "Failed: fstat" << std::endl;
		exit(-1);
	}
	/*
	* �t�@�C������������Ƀ}�b�v���Abase_addr�ɑ��
	* mmap�֐��̈����ɓn���}�b�v�T�C�Y��stat�\���̂̃����o��st_size���g�p����B
	*/
	u8 *base_addr = (u8 *)mmap(NULL, file_info->st_size, PROT_READ, MAP_PRIVATE, fd, 0);

	/*
	* ELF�w�b�_�\���̂̓t�@�C�����}�b�v���ꂽ�擪�A�h���X�ƑΉ����Ă���̂ŁA�L���X�g���Ă��̂܂ܑ��
	*/
	this->elf_header = (Elf64_Ehdr *)base_addr;

	/*
	* ELF�w�b�_�̂��Ƃ����Ƀv���O�����w�b�_������̂ŁA
	* Elf_Ehdr�\���̂̃T�C�Y���I�t�Z�b�g�𑫂��đ��
	*/
	this->elf_program_header = (Elf64_Phdr *)(base_addr + sizeof(Elf64_Ehdr));

	// �t�@�C���N���[�Y
	close(fd);
}