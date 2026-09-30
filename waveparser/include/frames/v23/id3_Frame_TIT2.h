#pragma once
#include "id3_Frame_Text.h"

namespace WAVE
{
	struct ID3_Frame_TIT2 : public ID3_TextFrame_T
	{
		std::string get_name() const { return "TIT2"; }

		std::string get_description() const { return "Title/songname/content description"; }
	};
} // namespace WAVE