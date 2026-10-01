#pragma once

#include "Core.h"

namespace waveparser
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
		uint32_t Size = 0;
		uint16_t Flags;
	};
#pragma pack(pop)

	struct ID3Frame
	{

		virtual ~ID3Frame() = default;

		virtual void ProcessData(std::ifstream &stream, const ID3FrameHeader &header) = 0;

		virtual ETagType GetTagType() const = 0;

		virtual std::string GetDescription() const = 0;

		virtual std::string GetValue() const = 0;

		std::string ToString() const { return GetDescription() + " : " + GetValue(); };
	};

} // namespace waveparser
