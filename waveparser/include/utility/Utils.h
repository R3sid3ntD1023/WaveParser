#pragma once

#include "Core.h"

namespace WAVE::utils
{
	template <typename T>
	static T byte_array_to_int(const byte_t b[4])
	{
		return b[3] | b[2] << 8 | b[1] << 16 | b[0] << 24;
	}

	template <typename T>
	static T byte_array_to_int_BE(const byte_t b[4])
	{
		return b[0] | b[1] << 8 | b[2] << 16 | b[3] << 24;
	}

	static uint32_t from_little_endian(uint32_t val)
	{
#if __BYTE_ORDER__ == __ORDER_LITTLE_ENDIAN__
		return val;
#else

		return ((val & 0x000000FFU) << 24) | ((val & 0x0000FF00U) << 8) | ((val & 0x00FF0000U) >> 8) | ((val & 0xFF000000U) >> 24);
#endif
	}

	static uint32_t from_big_endian(uint32_t val)
	{
		return ((val & 0x000000FF) << 24) | ((val & 0x0000FF00) << 8) | ((val & 0x00FF0000) >> 8) | ((val & 0xFF000000) >> 24);
	}

	static uint16_t from_big_endian(uint16_t val)
	{
		return ((val & 0x00FF) << 8) | ((val & 0xFF00) >> 8);
	}

	static uint32_t decode_synh_safe(const byte_t b[4])
	{
		return (b[0] << 21) | (b[1] << 14) | (b[2] << 7) | b[3];
	}
} // namespace WAVE::utils