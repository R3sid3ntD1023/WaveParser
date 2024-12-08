#pragma once

#include "id3_Frame.h"

namespace WAVE
{

	enum text_encoding : byte_t
	{
		ISO_8859_1,
		ISO_IEC_10646_1_1993, // unicode,
		UTF_16BE,
		UTF_8
	};

	struct Frame_IMPL
	{
		byte_t size[4];
		id3_Frame_t::e_frame_flags flags[2];
		text_encoding encoding;
	};

	struct id3_TextFrame_t : public id3_Frame_t
	{
		Frame_IMPL Impl;
		std::string text;

		void process_data(std::ifstream &stream);

		unsigned get_size();

		virtual std::string to_string() const { return text; }
	};
}