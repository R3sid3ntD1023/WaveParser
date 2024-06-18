#pragma once
#include "id3_Frame.h"

#define TXXX 0x54585858

enum text_encoding : uint8_t
{
	ISO_8859_1,
	ISO_IEC_10646_1_1993, //unicode,
	UTF_16BE,
	UTF_8
};

struct Frame_IMPL
{
	uint8_t size[4];
	id3_Frame_t::e_frame_flags flags[2];
	text_encoding encoding;
};

struct id3_TextFrame_t : public id3_Frame_t
{
	Frame_IMPL Impl;
	std::string text;

	void process_data(FILE* f);

	unsigned get_size();

	virtual void print();
};
