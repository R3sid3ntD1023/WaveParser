#pragma once

#include "id3_Frame_Text.h"

namespace WAVE
{
#pragma pack(push, 1)

	struct TXXX_Header
	{
		ETextEncoding encoding;
	};
#pragma pack(pop)

	struct ID3_Frame_TXXX : public ID3_Frame_T
	{
		TXXX_Header Header;

		MultiString Text;

		void process_data(std::ifstream &stream, const ID3FrameHeader &h) override;

		std::string get_name() const { return Text.Description; }

		std::string get_description() const { return Text.Description; }

		std::string get_value() const override { return Text.Value; }
	};
} // namespace WAVE