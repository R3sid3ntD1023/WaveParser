#pragma once

#include "id3_Frame.h"
#include "utility/Utils.h"

namespace WAVE
{

	enum ETextEncoding : byte_t
	{
		ISO_8859_1 = 0x00,
		ISO_IEC_10646_1_1993 = 0x01, // UTF-16 with BOM,
		UTF_16BE = 0x02,			 // UTF-16 big endian without BOM
		UTF_8 = 0x03
	};

	struct ID3_TextFrame_T : public ID3_Frame_T
	{
		std::string text;

		void process_data(std::ifstream &stream, const ID3FrameHeader &header);

		std::string get_value() const override { return text; }
	};

	struct MultiString
	{
		std::string Description;
		std::string Value;
	};

	MultiString parseMultiString(std::ifstream &stream, ETextEncoding encoding, uint32_t size);
} // namespace WAVE