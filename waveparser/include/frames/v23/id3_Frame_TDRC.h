#pragma once

#include "id3_Frame_Text.h"

namespace WAVE
{
	struct ID3_Frame_TDRC : public ID3_TextFrame_T
	{
		std::string get_name() const { return "TDRC"; }

		std::string get_description() const { return "RecordingTime"; }
	};
} // namespace WAVE