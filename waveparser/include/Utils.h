#pragma once

#include "Core.h"

struct utils
{
	static unsigned char_array_to_unsigned(const char byte_t[4]);

	static int byte_array_to_int(const byte_t byte_t[4]);

	static int byte_array_to_int_BE(const byte_t byte_t[4]);

	static unsigned byte_array_to_unsigned(const byte_t byte_t[4]);
};