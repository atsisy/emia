#pragma once

#include "types.hpp"

/**
* rflags���W�X�^�[�p���p��
*/
union RflagsRegister {

	/*
	* �ׂ���bit�ŃA�N�Z�X
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
	* 64bit�A�N�Z�X�p
	*/
	u64 rflags;

	RflagsRegister() { this->rflags = 0; }
};

constexpr u16 GPR_COUNT = 15;

class Registers {

private:
	/*
	* General Purpose Register
	* ���W�X�^ID�ŃA�N�Z�X����B
	* �f�[�^���������̓|�C���^����Q�Ƃ��ď���������
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
	* read_register64���\�b�h
	* ���W�X�^�̃f�[�^��l�n���Ŏ擾���邽�߂̃��\�b�h
	* ����
	* u16 register_id: ���W�X�^��ID
	* �Ԃ�l
	* �w�肵�����W�X�^�̃f�[�^�i�l�n���j
	*/
	u64 read_register64(u16 register_id);

	/*
	* ref_register64���\�b�h
	* ���W�X�^�̃f�[�^�ւ̃|�C���^���擾���邽�߂̃��\�b�h
	* ����
	* u16 register_id: ���W�X�^��ID
	* �Ԃ�l
	* �w�肵�����W�X�^�̃f�[�^�ւ̃|�C���^
	*/
	u64 *ref_register64(u16 register_id);

	void print_status();
};