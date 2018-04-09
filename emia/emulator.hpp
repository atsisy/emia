#pragma once

#include "register.hpp"
#include "elf_parser.hpp"

class Emulator {
public:
	/*
	* レジスタ
	*/
	Registers registers;

	/*
	* メモリ
	*/
	u8 *memory;

	/*
	* コンストラクタ
	* 引数
	* u64 mem_size: 確保するメモリのサイズ
	* ElfParser & parser: ELF実行ファイルを解析したELFパーサ
	* u64 rsp: スタックポインタの初期値を指定する
	*/
	Emulator(u64 mem_size, ElfParser & parser, u64 rsp);

	~Emulator();
};