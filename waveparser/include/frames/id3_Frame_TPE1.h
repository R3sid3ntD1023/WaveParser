#pragma once

#include "id3_Frame_Text.h"

namespace WAVE
{

	struct id3_Frame_TPE1 : public id3_TextFrame_t
	{
		std::string get_name() const { return "TPE1"; }
		std::string get_description() const { return "Lead performer(s)/Soloist(s)"; }
	};
}