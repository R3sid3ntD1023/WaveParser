#include "Utils.h"

unsigned utils::char_array_to_unsigned(const char byte_t[4])
{
	return byte_t[3] | byte_t[2] << 8 | byte_t[1] << 16 | byte_t[0] << 24;
}

int utils::byte_array_to_int(const byte_t byte_t[4])
{
	return byte_t[3] | byte_t[2] << 8 | byte_t[1] << 16 | byte_t[0] << 24;
}

int utils::byte_array_to_int_BE(const byte_t byte_t[4])
{
	return byte_t[0] | byte_t[1] << 8 | byte_t[2] << 16 | byte_t[3] << 24;
}

unsigned utils::byte_array_to_unsigned(const byte_t byte_t[4])
{
	return byte_t[3] | byte_t[2] << 8 | byte_t[1] << 16 | byte_t[0] << 24;
}
