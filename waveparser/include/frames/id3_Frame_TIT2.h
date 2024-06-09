#pragma once
#include "id3_Frame_Text.h"

#define TIT2 0x54495432

struct id3_Frame_TIT2 : public id3_TextFrame_t 
{
	const char* get_name() const { return "TIT2"; }
	const char* get_description() const { return "Title/songname/content description"; }
};