#pragma once

#include "Core.h"

namespace WAVE
{

	enum EFrameFlags : uint16_t
	{
		TagAlterPreservation = 0,
		FileAlterPreservation,
		ReadOnly,
		Compression,
		Encryption,
		GroupingIdentity
	};

#pragma pack(push, 1)
	struct ID3FrameHeader
	{
		uint32_t size = 0;
		uint16_t flags;
	};
#pragma pack(pop)

	struct ID3_Frame_T
	{

		virtual ~ID3_Frame_T() = default;

		virtual void process_data(std::ifstream &stream, const ID3FrameHeader &header) = 0;

		virtual std::string get_name() const = 0;

		virtual std::string get_description() const = 0;

		virtual std::string get_value() const = 0;

		std::string to_string() const { return get_name() + ":" + get_description() + "-" + get_value(); };
	};

} // namespace WAVE
