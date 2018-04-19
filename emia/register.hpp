#pragma once

#include "types.hpp"

/**
* rflagsレジスター用共用体
*/
union RflagsRegister {

	/*
	* 細かくbitでアクセス
	*/
	struct {
		u32 reserved_upper_32bit : 32;
		u16 reserved : 10;
		u8 id : 1;
		u8 vip : 1;
		u8 vif : 1;
		u8 ac : 1;
		u8 vm : 1;
		u8 rf : 1;
		u8 bit14 : 1;
		u8 nt : 1;
		u8 iopl : 2;
		u8 of : 1;
		u8 df : 1;
		u8 intf : 1;
		u8 tf : 1;
		u8 sf : 1;
		u8 zf : 1;
		u8 bit5 : 1;
		u8 af : 1;
		u8 bit3 : 1;
		u8 pf : 1;
		u8 bit1 : 1;
		u8 cf : 1;
	};

	/*
	* 64bitアクセス用
	*/
	u64 rflags;

	u64 mask_without_sf();

	void print_status();

	RflagsRegister() { this->rflags = 0; }
};

/*
* ModR_M共用体
* ModR/Mバイト
*/
union ModR_M {
	struct {
		u8 r_m : 3;
		u8 reg : 3;
		u8 mod : 2;
	};

	u8 modr_m;

	ModR_M(u8 byte);
};

constexpr u16 GPR_COUNT = 16;

class Registers {

private:
	/*
	* General Purpose Register
	* レジスタIDでアクセスする。
	* データ書き換えはポインタから参照して書き換える
	*/
	u64 gp_registers[GPR_COUNT];

public:
	// rflags register
	RflagsRegister rflags;
	
	// rip register
	u64 rip;

	Registers(u64 rip);
	~Registers();

	/*
	* read_register64メソッド
	* レジスタのデータを値渡しで取得するためのメソッド
	* 引数
	* u16 register_id: レジスタのID
	* 返り値
	* 指定したレジスタのデータ（値渡し）
	*/
	u64 read_register64(u16 register_id);

	/*
	* ref_register64メソッド
	* レジスタのデータへのポインタを取得するためのメソッド
	* 引数
	* u16 register_id: レジスタのID
	* 返り値
	* 指定したレジスタのデータへのポインタ
	*/
	u64 *ref_register64(u16 register_id);

	void print_status();
};