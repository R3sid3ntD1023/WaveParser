#pragma once

#include "Core.h"

namespace waveparser::utilities
{
	template <typename T>
	static T ByteArrayToInt(const byte_t bytes[4])
	{
		return bytes[3] | bytes[2] << 8 | bytes[1] << 16 | bytes[0] << 24;
	}

	template <typename T>
	static T ByteArrayToIntBE(const byte_t bytes[4])
	{
		return bytes[0] | bytes[1] << 8 | bytes[2] << 16 | bytes[3] << 24;
	}

	static uint32_t FromLittleEndian(uint32_t value)
	{
#if __BYTE_ORDER__ == __ORDER_LITTLE_ENDIAN__
		return value;
#else

		return ((value & 0x000000FFU) << 24) | ((value & 0x0000FF00U) << 8) | ((value & 0x00FF0000U) >> 8) | ((value & 0xFF000000U) >> 24);
#endif
	}

	static uint16_t FromLittleEndian(uint16_t value)
	{
#if __BYTE_ORDER__ == __ORDER_LITTLE_ENDIAN__
		return value;
#else

		return ((value & 0x0FFU) << 8) | ((value & 6528U) >> 8);
#endif
	}

	static uint32_t FromBigEndian(uint32_t value)
	{
		return ((value & 0x000000FF) << 24) | ((value & 0x0000FF00) << 8) | ((value & 0x00FF0000) >> 8) | ((value & 0xFF000000) >> 24);
	}

	static uint16_t FromBigEndian(uint16_t value)
	{
		return ((value & 0x00FF) << 8) | ((value & 0xFF00) >> 8);
	}

	static uint32_t DecodeSynchsafe(const byte_t bytes[4])
	{
		return (bytes[0] << 21) | (bytes[1] << 14) | (bytes[2] << 7) | bytes[3];
	}
} // namespace waveparser::utilities