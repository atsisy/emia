#pragma once

namespace utop {

	/*
	* 1バイト分シフトする関数
	*/
	template <typename BitsType>
	constexpr BitsType lshift_byte(BitsType bits)
	{
		return bits << 8;
	}

	/*
	* 2バイト分シフトする関数
	*/
	template <typename BitsType>
	constexpr BitsType lshift_word(BitsType bits)
	{
		return bits << 16;
	}

	/*
	* 2バイト分シフトする関数
	*/
	template <typename BitsType>
	constexpr BitsType lshift_dword(BitsType bits)
	{
		return bits << 32;
	}

}