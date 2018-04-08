#pragma once

#include "types.hpp"

union RflagsRegister {
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
	u64 rflags;

	RflagsRegister() { this->rflags = 0; }
};

constexpr u16 GPR_COUNT = 15;

class Registers {

private:
	u64 registers[GPR_COUNT];

public:
	RflagsRegister rflags;
	u64 rip;

	Registers(u64 rip);
	~Registers();

	u64 read_register64(u16 register_id);
	u64 *ref_register64(u16 register_id);
};