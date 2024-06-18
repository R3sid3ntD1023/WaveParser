#include "Utils.h"

unsigned utils::char_array_to_unsigned(const char byte[4])
{
	return byte[3] | byte[2] << 8 | byte[1] << 16 | byte[0] << 24;
}

int utils::byte_array_to_int(const uint8_t byte[4])
{
	return byte[3] | byte[2] << 8 | byte[1] << 16 | byte[0] << 24;
}

int utils::byte_array_to_int_BE(const uint8_t byte[4])
{
	return byte[0] | byte[1] << 8 | byte[2] << 16 | byte[3] << 24;
}

unsigned utils::byte_array_to_unsigned(const uint8_t byte[4])
{
	return byte[3] | byte[2] << 8 | byte[1] << 16 | byte[0] << 24;
}
