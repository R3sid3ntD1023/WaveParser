#pragma once
#include "id3_Frame_Text.h"

namespace waveparser
{
	struct ID3FrameTIT2 : public ID3TextFrame
	{
		std::string GetName() const { return "TIT2"; }

		std::string GetDescription() const { return "Title/songname/content description"; }
	};
} // namespace waveparser