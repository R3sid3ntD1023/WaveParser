#pragma once

#include "id3_Frame_Text.h"

namespace WAVE
{

	struct ID3_Frame_TPE1 : public ID3_TextFrame_T
	{
		std::string get_name() const { return "TPE1"; }

		std::string get_description() const { return "Lead performer(s)/Soloist(s)"; }
	};
} // namespace WAVE