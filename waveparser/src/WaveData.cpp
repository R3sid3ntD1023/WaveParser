#include "WaveData.h"


chunk_t::chunk_t(int size)
{
	header.size = size;
}

chunk_data_t::chunk_data_t(const chunk_data_t& other)
{
	data = new byte[other.header.size];
	memcpy(data, other.data, other.header.size);
}

chunk_data_t::chunk_data_t(int size)
	:chunk_t(size)
{
	data = new byte[size];
}

chunk_data_t::~chunk_data_t()
{
	if(data)
		delete[] data;
}

chunk_data_t& chunk_data_t::operator=(const chunk_data_t& rhs)
{
	header.size = rhs.header.size;
	data = new byte[rhs.header.size];
	memcpy(data, rhs.data, rhs.header.size);
	return *this;
}
