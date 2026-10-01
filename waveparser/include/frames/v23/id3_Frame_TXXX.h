#pragma once

#include "id3_Frame_Text.h"

namespace waveparser
{
#pragma pack(push, 1)

	struct TXXXHeader
	{
		ETextEncoding Encoding;
	};
#pragma pack(pop)

	struct ID3FrameTXXX : public ID3Frame
	{
		TXXXHeader Header;

		MultiString Text;

		void ProcessData(std::ifstream &stream, const ID3FrameHeader &header) override;

		std::string GetName() const { return "TXXX"; }

		std::string GetDescription() const { return Text.Description; }

		std::string GetValue() const override { return Text.Value; }
	};
} // namespace waveparser