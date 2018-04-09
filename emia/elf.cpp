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
	* ファイル情報受け取りのための構造体を初期化
	*/
	file_info = new struct stat;

	// ファイルオープン。エラーならメッセージ出して終了
	int fd = open(path.c_str(), O_RDONLY);
	if (fd == -1) {
		std::cerr << path << ": File not found." << std::endl;
		exit(-1);
	}

	/*
	* 開いたファイルの情報を取得
	* エラーだったらメッセージ出して終了
	*/
	if (fstat(fd, file_info) == -1) {
		std::cerr << "Failed: fstat" << std::endl;
		exit(-1);
	}
	/*
	* ファイルをメモリ上にマップし、base_addrに代入
	* mmap関数の引数に渡すマップサイズはstat構造体のメンバのst_sizeを使用する。
	*/
	u8 *base_addr = (u8 *)mmap(NULL, file_info->st_size, PROT_READ, MAP_PRIVATE, fd, 0);

	/*
	* ELFヘッダ構造体はファイルがマップされた先頭アドレスと対応しているので、キャストしてそのまま代入
	*/
	this->elf_header = (Elf64_Ehdr *)base_addr;

	/*
	* ELFヘッダのあとすぐにプログラムヘッダが来るので、
	* Elf_Ehdr構造体のサイズ分オフセットを足して代入
	*/
	this->elf_program_header = (Elf64_Phdr *)(base_addr + sizeof(Elf64_Ehdr));

	// ファイルクローズ
	close(fd);
}