#pragma once

#include "Core.h"

typedef unsigned long long buffer_size;

struct Buffer
{
	buffer_size size = 0;
	byte* data = nullptr;

	Buffer() = default;
	Buffer(const Buffer& other)
	{
		if (other.size && other.data)
		{
			Allocate(other.size);
			memcpy_s(data, size, other.data, other.size);
		}
	}

	Buffer(buffer_size size)
	{
		Allocate(size);
	}

	~Buffer()
	{
		if (data) Release();
	}

	void Allocate(buffer_size size)
	{
		Release();

		this->size = size;
		data = new byte[size + 1];
		data[size] = 0;
	}

	void Release()
	{
		size = 0;
		delete[] data;
	}

	template<typename T>
	T* as() const
	{
		return (T*)data;
	}

	operator byte* () { return data; }


	Buffer& operator=(const Buffer& rhs)
	{
		if (rhs.size && rhs.data)
		{
			Allocate(rhs.size);
			memcpy_s(data, size, rhs.data, rhs.size);
		}

		return *this;
	}
};