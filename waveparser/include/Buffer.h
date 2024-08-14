#pragma once

#include "Core.h"

typedef unsigned long long buffer_size;

struct Buffer
{
	buffer_size Size = 0;
	byte_t* Data = nullptr;

	Buffer() = default;
	Buffer(const Buffer& other)
	{
		if (other.Size && other.Data)
		{
			Allocate(other.Size);
			memcpy_s(Data, Size, other.Data, other.Size);
		}
	}

	Buffer(buffer_size size)
	{
		Allocate(size);
	}

	~Buffer()
	{
		if (Data) Release();
	}

	void Allocate(buffer_size size)
	{
		Release();

		this->Size = size;
		Data = new byte_t[size + 1];
		Data[size] = 0;
	}

	void Release()
	{
		Size = 0;
		delete[] Data;
	}

	template<typename T>
	T* as() const
	{
		return (T*)Data;
	}

	operator byte_t* () { return Data; }


	Buffer& operator=(const Buffer& rhs)
	{
		if (rhs.Size && rhs.Data)
		{
			Allocate(rhs.Size);
			memcpy_s(Data, Size, rhs.Data, rhs.Size);
		}

		return *this;
	}
};