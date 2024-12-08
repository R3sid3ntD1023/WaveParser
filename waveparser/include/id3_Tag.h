#pragma once
#include "Core.h"

namespace WAVE
{
	struct id3_tag_t
	{
		byte_t id[4];

		bool operator==(const id3_tag_t &rhs) const { return strcmp(id, rhs.id); }
		bool operator!=(const id3_tag_t &rhs) const { return !(*this == rhs); }

		operator std::string() const { return std::string(id, 4); }
	};

}

namespace std
{
	template <>
	struct hash<WAVE::id3_tag_t>
	{
		size_t operator()(const WAVE::id3_tag_t &tag) const
		{
			return hash<string>()(tag);
		}
	};
}