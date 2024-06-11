#include "WaveData.h"


chunk_t::chunk_t(const chunk_info_t& info)
	:header(info)
{
	data = new byte[info.size];
}

chunk_t::chunk_t(const chunk_t& other)
{
	header = other.header;
	data = new byte[other.header.size];
	memcpy(data, other.data, other.header.size);
}

chunk_t::~chunk_t()
{
	delete[] data;
}

chunk_t& chunk_t::operator=(const chunk_t& rhs)
{
	header = rhs.header;
	data = new byte[rhs.header.size];
	memcpy(data, rhs.data, rhs.header.size);
	return *this;
}

