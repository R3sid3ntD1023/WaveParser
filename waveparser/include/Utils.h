#pragma once

#include "Core.h"

struct utils
{
	static unsigned char_array_to_unsigned(const char byte[4]);

	static int byte_array_to_int(const uint8_t byte[4]);

	static int byte_array_to_int_BE(const uint8_t byte[4]);

	static unsigned byte_array_to_unsigned(const uint8_t byte[4]);
};