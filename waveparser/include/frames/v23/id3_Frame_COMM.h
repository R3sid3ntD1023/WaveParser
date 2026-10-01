#pragma once
#include "id3_Frame_Text.h"

namespace waveparser
{
#pragma pack(push, 1)
	struct COMMHeader
	{
		ETextEncoding Encoding;
		byte_t Language[3];
	};
#pragma pack(pop)

	struct ID3FrameCOMM : public ID3Frame
	{
		COMMHeader Header;
		MultiString Text;

		void ProcessData(std::ifstream &stream, const ID3FrameHeader &header) override;

		ETagType GetTagType() const override { return ETagType::COMM; }

		std::string GetDescription() const { return Text.Description.empty() ? "Comment" : Text.Description; }

		std::string GetValue() const override { return Text.Value; }
	};
} // namespace waveparser