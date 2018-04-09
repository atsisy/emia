#pragma once

#include <string>
#include <elf.h>
#include <x86_64-linux-gnu/sys/stat.h>
#include "types.hpp"

class ElfParser {

public:
	/*
	* ELFヘッダのポインタかつ、ファイルがマップされたメモリのアドレス
	* これら２つのメモリアドレスは同一になるため共用体として定義する。
	*/
	union
	{
		const u8 *mapped_addr;
		Elf64_Ehdr *elf_header;
	};

	/*
	* ELFにおけるプログラムヘッダのメモリアドレス
	*/
	Elf64_Phdr *elf_program_header;

	/*
	* ファイル情報
	* statシステムコールで取得したもの
	*/
	struct stat *file_info;

	/*
	* コンストラクタ
	* 引数
	* ELF形式の実行ファイルに対するパス
	*/
	ElfParser(std::string path);

};