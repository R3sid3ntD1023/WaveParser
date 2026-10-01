#pragma once

#include <algorithm>
#include <bit>
#include <filesystem>
#include <fstream>
#include <functional>
#include <iostream>
#include <memory>
#include <sstream>
#include <stdint.h>
#include <string>
#include <unordered_map>

namespace waveparser
{
	using byte_t = unsigned char;

	enum class ETagType : uint8_t
	{
		Unknown,
		COMM,
		TDRC,
		TIT2,
		TPE1,
		TXXX,
	};

	inline ETagType GetTagTypeFromString(const std::string &str)
	{
		if (str == "COMM")
			return ETagType::COMM;
		if (str == "TDRC")
			return ETagType::TDRC;
		if (str == "TIT2")
			return ETagType::TIT2;
		if (str == "TPE1")
			return ETagType::TPE1;
		if (str == "TXXX")
			return ETagType::TXXX;
		return ETagType::Unknown;
	}

	inline std::string ToString(ETagType type)
	{
		switch (type)
		{
		case ETagType::COMM:
			return "COMM";
		case ETagType::TDRC:
			return "TDRC";
		case ETagType::TIT2:
			return "TIT2";
		case ETagType::TPE1:
			return "TPE1";
		case ETagType::TXXX:
			return "TXXX";
		default:
			return "Unknown";
		}
	}
} // namespace waveparser