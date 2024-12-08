#pragma once

#include "id3_Frame_Text.h"

namespace WAVE
{

	struct id3_Frame_TDRC : public id3_TextFrame_t
	{
		std::string get_name() const { return "TDRC"; }
		std::string get_description() const { return "RecordingTime"; }
	};
}