#pragma once
#include "id3_Frame_Text.h"

namespace WAVE
{

	struct id3_Frame_TIT2 : public id3_TextFrame_t
	{
		std::string get_name() const { return "TIT2"; }

		std::string get_description() const { return "Title/songname/content description"; }
	};
}