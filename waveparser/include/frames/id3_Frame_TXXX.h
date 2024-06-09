#pragma once
#include "id3_Frame.h"
#include "id3_Frame_Text.h"

struct id3_Frame_TXXX : public id3_Frame_t
{
	~id3_Frame_TXXX();

	Frame_IMPL DescritpionImpl;
	std::string Desciption;
	std::string Value;

	const char* get_name() const { return Desciption.c_str(); }
	const char* get_description() const { return "user defined text information frame"; }

	void process_data(FILE* f);

	void print();
};
