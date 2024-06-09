#pragma once
#include "id3_Frame_Text.h"

#define COMM 0x434F4D4D

struct id3_Frame_COMM : public id3_TextFrame_t 
{
	std::string get_name() const { return "COMM"; }
	std::string get_description() const { return "Comments"; }
};