#pragma once
#include "Core.h"

struct id3_tag_t
{
	byte id[4];

	bool operator==(const id3_tag_t& rhs) const { return strcmp(id, rhs.id); }
	bool operator!=(const id3_tag_t& rhs) const { return !(*this == rhs); }
};

namespace std
{
	template<>
	struct hash<id3_tag_t>
	{
		size_t operator()(const id3_tag_t& tag) const
		{
			char str[sizeof(byte) * 4 + 1];
			str[4] = '\0';
			memcpy(str, tag.id, 4);
			return hash<string>()(str);
		}
	};
}

