#pragma once

#include "id3_Frame.h"
#include "id3_Frame_Text.h"

namespace WAVE
{

	struct id3_Frame_TXXX : public id3_Frame_t
	{
		~id3_Frame_TXXX();

		Frame_IMPL DescritpionImpl;
		std::string Desciption;
		std::string Value;

		std::string get_name() const { return Desciption.c_str(); }
		std::string get_description() const { return "user defined text information frame"; }

		void process_data(std::ifstream &stream);
		virtual std::string to_string() const;
	};
}