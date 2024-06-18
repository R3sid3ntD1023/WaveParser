#pragma once

#include "Core.h"

struct utils
{
	static unsigned char_array_to_unsigned(const char byte[4]);

	static int byte_array_to_int(const byte byte[4]);

	static int byte_array_to_int_BE(const byte byte[4]);

	static unsigned byte_array_to_unsigned(const byte byte[4]);
};