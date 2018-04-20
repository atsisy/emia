#pragma once

namespace utop {

	/*
	* 1�o�C�g���V�t�g����֐�
	*/
	template <typename BitsType>
	constexpr BitsType lshift_byte(BitsType bits)
	{
		return bits << 8;
	}

	/*
	* 2�o�C�g���V�t�g����֐�
	*/
	template <typename BitsType>
	constexpr BitsType lshift_word(BitsType bits)
	{
		return bits << 16;
	}

	/*
	* 2�o�C�g���V�t�g����֐�
	*/
	template <typename BitsType>
	constexpr BitsType lshift_dword(BitsType bits)
	{
		return bits << 32;
	}

}