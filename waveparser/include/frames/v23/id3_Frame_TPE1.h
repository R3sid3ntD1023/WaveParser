#pragma once

#include "id3_Frame_Text.h"

namespace waveparser
{

	struct ID3FrameTPE1 : public ID3TextFrame
	{
		std::string GetName() const { return "TPE1"; }

		std::string GetDescription() const { return "Lead performer(s)/Soloist(s)"; }
	};
} // namespace waveparser