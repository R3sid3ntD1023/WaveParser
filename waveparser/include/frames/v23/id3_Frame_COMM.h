#pragma once
#include "id3_Frame_Text.h"

namespace WAVE
{
#pragma pack(push, 1)
	struct COMM_Header
	{
		ETextEncoding encoding;
		byte_t language[3];
	};
#pragma pack(pop)

	struct ID3_Frame_COMM : public ID3_Frame_T
	{
		COMM_Header Header;
		MultiString Text;

		void process_data(std::ifstream &stream, const ID3FrameHeader &h) override;

		std::string get_name() const { return "COMM"; }

		std::string get_description() const { return Text.Description; }

		std::string get_value() const override { return Text.Value; }
	};
} // namespace WAVE